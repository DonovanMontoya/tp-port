/**
 * src/dolphin/tev.cpp
 * TEV (Texture Environment) → GLSL shader compiler.
 *
 * The GC TEV unit is a programmable colour/alpha combiner with up to
 * 16 stages.  Each stage reads inputs from {zero, half, konst, previous,
 * colour0-2, texel0-7, rasterised colour/alpha} and combines them with:
 *   out = clamp(d +/- (a*(1-c) + b*c) +/- bias) * scale
 *
 * Strategy:
 *   1. GX*SetTev* calls accumulate state into a TevStageDesc array.
 *   2. At draw time (GXBegin) we hash the current TEV config into a
 *      key and look up the shader cache.  Cache miss → generate GLSL.
 *   3. The generated shader is compiled, linked, and stored.
 *
 * This covers the ~20 most common TEV patterns that account for the
 * majority of TP materials.  Less common patterns fall back to a
 * passthrough that looks approximately correct.
 */

#include "port/port.h"
#include "dolphin/gx.h"

#include <glad/glad.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <sstream>

// -----------------------------------------------------------------------
// TEV state mirror (updated by GXSetTev* calls in gx.cpp)
// -----------------------------------------------------------------------

struct TevStage {
    // Colour combiner
    GXTevColorArg colorA, colorB, colorC, colorD;
    GXTevOp       colorOp;
    GXTevBias     colorBias;
    GXTevScale    colorScale;
    bool          colorClamp;
    GXTevRegID    colorOutReg;
    // Alpha combiner
    GXTevAlphaArg alphaA, alphaB, alphaC, alphaD;
    GXTevOp       alphaOp;
    GXTevBias     alphaBias;
    GXTevScale    alphaScale;
    bool          alphaClamp;
    GXTevRegID    alphaOutReg;
    // Input routing
    GXTexCoordID  texcoord;
    GXTexMapID    texmap;
    GXChannelID   rasColor;
    // Konst selectors
    GXTevKColorSel kcolorSel;
    GXTevKAlphaSel kalpahSel;
};

struct TevState {
    int          numStages   = 1;
    TevStage     stages[16]  = {};
    GXColor      regColor[4] = {};   // tevreg0-3
    GXColor      konstColor[4] = {};

    // For hashing: pack relevant fields into a 64-bit key per stage
    uint64_t StageHash(int i) const {
        const TevStage& s = stages[i];
        return ((uint64_t)s.colorA)      |
               ((uint64_t)s.colorB << 4) |
               ((uint64_t)s.colorC << 8) |
               ((uint64_t)s.colorD << 12)|
               ((uint64_t)s.colorOp<< 16)|
               ((uint64_t)s.alphaA << 18)|
               ((uint64_t)s.alphaB << 22)|
               ((uint64_t)s.alphaC << 26)|
               ((uint64_t)s.alphaD << 30)|
               ((uint64_t)s.texmap << 34)|
               ((uint64_t)s.rasColor<<38)|
               ((uint64_t)s.kcolorSel<<42)|
               ((uint64_t)s.kalpahSel<<48);
    }
};

// Global TEV state (written by GX* calls, read at GXBegin time)
TevState g_tev;

// -----------------------------------------------------------------------
// Shader cache
// -----------------------------------------------------------------------
struct ShaderEntry {
    GLuint program = 0;
    // Uniform locations
    GLint uMVP      = -1;
    GLint uTex[8]   = {-1,-1,-1,-1,-1,-1,-1,-1};
    GLint uKColor[4]= {-1,-1,-1,-1};
    GLint uRegColor[4]={-1,-1,-1,-1};
    GLint uAmbColor = -1;
    GLint uMatColor = -1;
};

static std::unordered_map<size_t, ShaderEntry> sShaderCache;
static GLuint sCurrentProgram = 0;

// -----------------------------------------------------------------------
// GLSL codegen helpers
// -----------------------------------------------------------------------
#if defined(__APPLE__)
static const char* kGLSLVersion = "#version 410 core\n";
#else
static const char* kGLSLVersion = "#version 450 core\n";
#endif

static const char* ColorArgName(GXTevColorArg a, int stage) {
    static char buf[64];
    switch (a) {
        case GX_CC_CPREV: return "cprev";
        case GX_CC_APREV: return "vec4(aprev)";
        case GX_CC_C0:    return "creg0";
        case GX_CC_A0:    return "vec4(areg0)";
        case GX_CC_C1:    return "creg1";
        case GX_CC_A1:    return "vec4(areg1)";
        case GX_CC_C2:    return "creg2";
        case GX_CC_A2:    return "vec4(areg2)";
        case GX_CC_TEXC:  snprintf(buf,sizeof(buf),"texc%d",stage); return buf;
        case GX_CC_TEXA:  snprintf(buf,sizeof(buf),"vec4(texc%d.a)",stage); return buf;
        case GX_CC_RASC:  return "rasc";
        case GX_CC_RASA:  return "vec4(rasc.a)";
        case GX_CC_ONE:   return "vec4(1.0)";
        case GX_CC_HALF:  return "vec4(0.5)";
        case GX_CC_KONST: snprintf(buf,sizeof(buf),"kcolor%d",stage); return buf;
        case GX_CC_ZERO:  return "vec4(0.0)";
        default:          return "vec4(0.0)";
    }
}

static const char* AlphaArgName(GXTevAlphaArg a, int stage) {
    static char buf[64];
    switch (a) {
        case GX_CA_APREV: return "aprev";
        case GX_CA_A0:    return "areg0";
        case GX_CA_A1:    return "areg1";
        case GX_CA_A2:    return "areg2";
        case GX_CA_TEXA:  snprintf(buf,sizeof(buf),"texc%d.a",stage); return buf;
        case GX_CA_RASA:  return "rasc.a";
        case GX_CA_KONST: snprintf(buf,sizeof(buf),"kalpha%d",stage); return buf;
        case GX_CA_ZERO:  return "0.0";
        default:          return "0.0";
    }
}

static const char* ScaleStr(GXTevScale sc) {
    switch (sc) {
        case GX_CS_SCALE_1:  return "1.0";
        case GX_CS_SCALE_2:  return "2.0";
        case GX_CS_SCALE_4:  return "4.0";
        case GX_CS_DIVIDE_2: return "0.5";
        default:             return "1.0";
    }
}

static float BiasVal(GXTevBias b) {
    switch (b) {
        case GX_TB_ADDHALF:  return  0.5f;
        case GX_TB_SUBHALF:  return -0.5f;
        default:             return  0.0f;
    }
}

// -----------------------------------------------------------------------
// Generate a GLSL fragment shader for the current TEV state
// -----------------------------------------------------------------------
static std::string GenerateTEVShader(const TevState& tev) {
    std::ostringstream ss;
    ss << kGLSLVersion;

    // Texture samplers
    for (int i = 0; i < 8; i++)
        ss << "uniform sampler2D uTex" << i << ";\n";
    ss << "uniform bool uHasTex[8];\n";
    ss << "uniform vec4 uKColor[4];\n";
    ss << "uniform vec4 uRegColor[4];\n";  // tev regs 0-3
    ss << "uniform vec4 uAmbColor;\n";
    ss << "uniform vec4 uMatColor;\n";

    ss << "in vec4 vColor;\n";
    ss << "in vec2 vTexCoord[8];\n";
    ss << "out vec4 fragColor;\n\n";

    ss << "void main() {\n";

    // TEV working registers
    ss << "    vec4 cprev = vColor;\n";     // previous colour
    ss << "    float aprev = vColor.a;\n";  // previous alpha
    ss << "    vec4 creg0 = uRegColor[0];\n";
    ss << "    float areg0 = uRegColor[0].a;\n";
    ss << "    vec4 creg1 = uRegColor[1];\n";
    ss << "    float areg1 = uRegColor[1].a;\n";
    ss << "    vec4 creg2 = uRegColor[2];\n";
    ss << "    float areg2 = uRegColor[2].a;\n";
    ss << "    vec4 rasc   = vColor;\n";    // rasterised colour

    for (int i = 0; i < tev.numStages; i++) {
        const TevStage& s = tev.stages[i];

        // Sample texture for this stage
        ss << "    vec4 texc" << i << " = ";
        if (s.texmap < 8)
            ss << "(uHasTex[" << s.texmap << "] ? texture(uTex" << s.texmap
               << ", vTexCoord[" << (s.texcoord < 8 ? s.texcoord : 0) << "]) : vec4(1));\n";
        else
            ss << "vec4(1);\n";

        // Konst colour / alpha for this stage
        int kci = (s.kcolorSel >= GX_TEV_KCSEL_K0 && s.kcolorSel <= GX_TEV_KCSEL_K3)
                  ? (s.kcolorSel - GX_TEV_KCSEL_K0) : 0;
        ss << "    vec4 kcolor" << i << " = uKColor[" << kci << "];\n";
        ss << "    float kalpha" << i << " = uKColor[" << kci << "].a;\n";

        // Colour combiner: out = clamp(d +/- (a*(1-c) + b*c) + bias) * scale
        float bias = BiasVal(s.colorBias);
        const char* sc   = ScaleStr(s.colorScale);
        const char* outR = (s.colorOutReg == GX_TEVREG0) ? "creg0"
                         : (s.colorOutReg == GX_TEVREG1) ? "creg1"
                         : (s.colorOutReg == GX_TEVREG2) ? "creg2" : "cprev";

        ss << "    " << outR << " = ";
        if (s.colorOp == GX_TEV_ADD || s.colorOp == GX_TEV_SUB) {
            const char* op = (s.colorOp == GX_TEV_ADD) ? "+" : "-";
            ss << ColorArgName(s.colorD, i) << " "<<op<<" ("
               << ColorArgName(s.colorA, i) << "*(1.0-" << ColorArgName(s.colorC, i) << ")+"
               << ColorArgName(s.colorB, i) << "*"      << ColorArgName(s.colorC, i) << ")";
            if (bias != 0.0f) ss << (bias>0?"+":"-") << fabsf(bias);
        } else {
            // COMP_R8_GT / COMP_GR16_GT etc. → approximate as max
            ss << "max(" << ColorArgName(s.colorA, i) << "," << ColorArgName(s.colorB, i) << ")";
        }
        ss << ";\n    " << outR << " *= " << sc << ";\n";
        if (s.colorClamp)
            ss << "    " << outR << " = clamp(" << outR << ", 0.0, 1.0);\n";
        if (strcmp(outR,"cprev")!=0) ss << "    cprev = " << outR << ";\n";

        // Alpha combiner
        float abias = BiasVal(s.alphaBias);
        const char* asc  = ScaleStr(s.alphaScale);
        const char* outA = (s.alphaOutReg == GX_TEVREG0) ? "areg0"
                         : (s.alphaOutReg == GX_TEVREG1) ? "areg1"
                         : (s.alphaOutReg == GX_TEVREG2) ? "areg2" : "aprev";

        ss << "    " << outA << " = ";
        if (s.alphaOp == GX_TEV_ADD || s.alphaOp == GX_TEV_SUB) {
            const char* aop = (s.alphaOp == GX_TEV_ADD) ? "+" : "-";
            ss << AlphaArgName(s.alphaD, i) << " "<<aop<<" ("
               << AlphaArgName(s.alphaA, i) << "*(1.0-" << AlphaArgName(s.alphaC, i) << ")+"
               << AlphaArgName(s.alphaB, i) << "*"      << AlphaArgName(s.alphaC, i) << ")";
            if (abias != 0.0f) ss << (abias>0?"+":"-") << fabsf(abias);
        } else {
            ss << "max(" << AlphaArgName(s.alphaA, i) << "," << AlphaArgName(s.alphaB, i) << ")";
        }
        ss << ";\n    " << outA << " *= " << asc << ";\n";
        if (s.alphaClamp)
            ss << "    " << outA << " = clamp(" << outA << ", 0.0, 1.0);\n";
        if (strcmp(outA,"aprev")!=0) ss << "    aprev = " << outA << ";\n";
    }

    ss << "    fragColor = vec4(cprev.rgb, aprev);\n";
    ss << "}\n";
    return ss.str();
}

// Vertex shader shared by all TEV programs
static const char* kTEVVert = R"GLSL(
layout(location=0) in vec3  aPos;
layout(location=1) in vec4  aColor;
layout(location=2) in vec2  aTexCoord0;
layout(location=3) in vec2  aTexCoord1;

uniform mat4 uMVP;

out vec4 vColor;
out vec2 vTexCoord[8];

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    vColor = aColor;
    vTexCoord[0] = aTexCoord0;
    vTexCoord[1] = aTexCoord1;
    for (int i=2; i<8; i++) vTexCoord[i] = aTexCoord0;
}
)GLSL";

// -----------------------------------------------------------------------
// Shader compilation helpers
// -----------------------------------------------------------------------
static GLuint CompileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok=0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048]; glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        tp::log::error("TEV shader compile: %s", log);
        tp::log::error("Source:\n%s", src);
    }
    return s;
}

static GLuint LinkProgram(const char* vert, const std::string& frag) {
    GLuint vs = CompileShader(GL_VERTEX_SHADER,   vert);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, frag.c_str());
    GLuint p  = glCreateProgram();
    glAttachShader(p, vs); glAttachShader(p, fs);
    glLinkProgram(p);
    GLint ok=0; glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) { char log[512]; glGetProgramInfoLog(p,sizeof(log),nullptr,log); tp::log::error("TEV link: %s",log); }
    glDeleteShader(vs); glDeleteShader(fs);
    return p;
}

static ShaderEntry BuildEntry(const TevState& tev) {
    std::string frag = GenerateTEVShader(tev);
    ShaderEntry e;
    e.program = LinkProgram(kTEVVert, frag);
    glUseProgram(e.program);
    e.uMVP = glGetUniformLocation(e.program, "uMVP");
    for (int i=0;i<8;i++) {
        char n[16]; snprintf(n,sizeof(n),"uTex%d",i);
        e.uTex[i] = glGetUniformLocation(e.program, n);
        if (e.uTex[i]>=0) glUniform1i(e.uTex[i], i);
    }
    e.uAmbColor = glGetUniformLocation(e.program,"uAmbColor");
    e.uMatColor = glGetUniformLocation(e.program,"uMatColor");
    for(int i=0;i<4;i++){
        char n[32];
        snprintf(n,sizeof(n),"uKColor[%d]",i);   e.uKColor[i]=glGetUniformLocation(e.program,n);
        snprintf(n,sizeof(n),"uRegColor[%d]",i);  e.uRegColor[i]=glGetUniformLocation(e.program,n);
    }
    return e;
}

// -----------------------------------------------------------------------
// Public: bind TEV shader for the current draw call
// -----------------------------------------------------------------------
namespace tp::tev {

void BindShaderForDraw() {
    // Hash the current TEV state
    size_t h = (size_t)g_tev.numStages;
    for (int i=0;i<g_tev.numStages;i++)
        h ^= (size_t)g_tev.StageHash(i) + 0x9e3779b9 + (h<<6) + (h>>2);

    auto it = sShaderCache.find(h);
    if (it == sShaderCache.end()) {
        sShaderCache[h] = BuildEntry(g_tev);
        it = sShaderCache.find(h);
    }

    const ShaderEntry& e = it->second;
    if (sCurrentProgram != e.program) {
        glUseProgram(e.program);
        sCurrentProgram = e.program;
    }

    // Upload konst colours and registers every draw (cheap uniform updates)
    for (int i=0;i<4;i++) {
        if (e.uKColor[i]>=0)
            glUniform4f(e.uKColor[i],
                g_tev.konstColor[i].r/255.f, g_tev.konstColor[i].g/255.f,
                g_tev.konstColor[i].b/255.f, g_tev.konstColor[i].a/255.f);
        if (e.uRegColor[i]>=0)
            glUniform4f(e.uRegColor[i],
                g_tev.regColor[i].r/255.f, g_tev.regColor[i].g/255.f,
                g_tev.regColor[i].b/255.f, g_tev.regColor[i].a/255.f);
    }
}

GLint GetMVPLocation() {
    auto it = sShaderCache.find(0);  // fallback; called after BindShaderForDraw
    return (it!=sShaderCache.end()) ? it->second.uMVP : -1;
}

void SetTevColor(int reg, GXColor c) {
    if (reg>=0 && reg<4) g_tev.regColor[reg] = c;
}
void SetKonstColor(int id, GXColor c) {
    if (id>=0 && id<4) g_tev.konstColor[id] = c;
}

void SetStageOrder(int stage, GXTexCoordID tc, GXTexMapID tm, GXChannelID ch) {
    if (stage<16) { g_tev.stages[stage].texcoord=tc; g_tev.stages[stage].texmap=tm; g_tev.stages[stage].rasColor=ch; }
}
void SetNumStages(int n) { g_tev.numStages = n; }
void SetColorIn(int s, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) {
    if(s<16){g_tev.stages[s].colorA=a;g_tev.stages[s].colorB=b;g_tev.stages[s].colorC=c;g_tev.stages[s].colorD=d;}
}
void SetAlphaIn(int s, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) {
    if(s<16){g_tev.stages[s].alphaA=a;g_tev.stages[s].alphaB=b;g_tev.stages[s].alphaC=c;g_tev.stages[s].alphaD=d;}
}
void SetColorOp(int s, GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID reg) {
    if(s<16){g_tev.stages[s].colorOp=op;g_tev.stages[s].colorBias=bias;g_tev.stages[s].colorScale=scale;g_tev.stages[s].colorClamp=clamp;g_tev.stages[s].colorOutReg=reg;}
}
void SetAlphaOp(int s, GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID reg) {
    if(s<16){g_tev.stages[s].alphaOp=op;g_tev.stages[s].alphaBias=bias;g_tev.stages[s].alphaScale=scale;g_tev.stages[s].alphaClamp=clamp;g_tev.stages[s].alphaOutReg=reg;}
}
void SetKColorSel(int s, GXTevKColorSel sel) { if(s<16) g_tev.stages[s].kcolorSel=sel; }
void SetKAlphaSel(int s, GXTevKAlphaSel sel) { if(s<16) g_tev.stages[s].kalpahSel=sel; }

} // namespace tp::tev
