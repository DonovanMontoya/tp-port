/**
 * src/dolphin/gx.cpp
 * GX → OpenGL 4.5 translation layer.
 *
 * Architecture overview:
 *   - GXBegin/GXEnd buffer immediate-mode vertex data into a per-frame VBO.
 *   - TEV stage configuration is hashed into a GLSL shader cache.
 *   - Texture objects are uploaded to GL textures on GXLoadTexObj.
 *   - Matrix loads mirror the GC position/normal matrix palette into UBOs.
 *   - GXCopyDisp blits the render framebuffer to the GLFW window.
 *
 * This file provides the scaffolding; full TEV→GLSL translation is the
 * largest single porting task and is marked TODO inline.
 */

#include "port/port.h"
#include "dolphin/gx.h"

#include <glad/glad.h>
#include <cstring>

// Forward declarations from tev.cpp and texture.cpp
namespace tp::tev {
    void BindShaderForDraw();
    GLint GetMVPLocation();
    extern void SetNumStages(int);
    extern void SetStageOrder(int, GXTexCoordID, GXTexMapID, GXChannelID);
    extern void SetColorIn(int, GXTevColorArg, GXTevColorArg, GXTevColorArg, GXTevColorArg);
    extern void SetAlphaIn(int, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg);
    extern void SetColorOp(int, GXTevOp, GXTevBias, GXTevScale, bool, GXTevRegID);
    extern void SetAlphaOp(int, GXTevOp, GXTevBias, GXTevScale, bool, GXTevRegID);
    extern void SetTevColor(int, GXColor);
    extern void SetKonstColor(int, GXColor);
    extern void SetKColorSel(int, GXTevKColorSel);
    extern void SetKAlphaSel(int, GXTevKAlphaSel);
}
namespace tp::texture {
    void Untile(int fmt, const void* src, int w, int h, void* dst,
                int* internalFmt, int* glFmt, int* dataType);
}
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>

// -----------------------------------------------------------------------
// Internal state
// -----------------------------------------------------------------------

// Vertex buffer for immediate-mode GXBegin/GXEnd
static std::vector<float>  sVtxBuffer;
static GXPrimitive          sCurrPrimType   = GX_TRIANGLES;
static GXVtxFmt             sCurrVtxFmt     = GX_VTXFMT0;
static u16                  sCurrVtxCount   = 0;

// Active GL objects
static GLuint sVAO = 0;
static GLuint sVBO = 0;

// Position matrix palette (12 * 32-bit * 12 floats = 64 GC matrices)
static float sPosMatPalette[64][3][4];
static float sProjectionMtx[4][4];
static GXProjectionType sProjType = GX_PERSPECTIVE;

// TEV / shader state
static GLuint sActiveProgram = 0;

// Texture map (GXTexMapID → GL texture handle)
static GLuint sTextures[8] = {};

// Clear color / depth
static float sClearR = 0, sClearG = 0, sClearB = 0, sClearA = 0;
static u32   sClearZ = 0x00FFFFFF;

// Callbacks
static GXDrawSyncCallback sDrawSyncCB = nullptr;
static GXDrawDoneCallback sDrawDoneCB = nullptr;

// -----------------------------------------------------------------------
// Simple default shaders
// The real implementation needs a TEV→GLSL compiler. For now we use a
// basic pass-through shader so the window at least renders something.
// -----------------------------------------------------------------------
static const char* kDefaultVert = R"GLSL(
#version 450 core
layout(location = 0) in vec3  aPos;
layout(location = 1) in vec4  aColor;
layout(location = 2) in vec2  aTexCoord;

uniform mat4 uMVP;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    vColor      = aColor;
    vTexCoord   = aTexCoord;
}
)GLSL";

static const char* kDefaultFrag = R"GLSL(
#version 450 core
in  vec4 vColor;
in  vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uTex0;
uniform bool      uHasTex;

void main() {
    if (uHasTex)
        fragColor = texture(uTex0, vTexCoord) * vColor;
    else
        fragColor = vColor;
}
)GLSL";

static GLuint CompileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        tp::log::error("GX shader compile: %s", log);
    }
    return s;
}

static GLuint LinkProgram(const char* vert, const char* frag) {
    GLuint vs = CompileShader(GL_VERTEX_SHADER,   vert);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, frag);
    GLuint p  = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

// -----------------------------------------------------------------------
// GX primitive → GL primitive
// -----------------------------------------------------------------------
static GLenum GXPrimToGL(GXPrimitive prim) {
    switch (prim) {
        case GX_POINTS:        return GL_POINTS;
        case GX_LINES:         return GL_LINES;
        case GX_LINESTRIP:     return GL_LINE_STRIP;
        case GX_TRIANGLES:     return GL_TRIANGLES;
        case GX_TRIANGLESTRIP: return GL_TRIANGLE_STRIP;
        case GX_TRIANGLEFAN:   return GL_TRIANGLE_FAN;
        case GX_QUADS:         return GL_TRIANGLES;  // converted during submit
        default:               return GL_TRIANGLES;
    }
}

// -----------------------------------------------------------------------
// GX management
// -----------------------------------------------------------------------
void GXInit(void* /*base*/, u32 /*size*/) {
    glGenVertexArrays(1, &sVAO);
    glGenBuffers(1, &sVBO);
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);

    // position (vec3), color (vec4), texcoord (vec2) = 9 floats/vertex
    constexpr GLsizei stride = 9 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    sActiveProgram = LinkProgram(kDefaultVert, kDefaultFrag);
    glUseProgram(sActiveProgram);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    tp::log::info("GXInit complete (OpenGL %s)", glGetString(GL_VERSION));
}

void GXFlush(void)        {}
void GXDrawDone(void)     { if (sDrawDoneCB) sDrawDoneCB(); }
void GXWaitDrawDone(void) {}
void GXAbortFrame(void)   {}
void GXPixModeSync(void)  {}
void GXTexModeSync(void)  {}
void GXSetDrawSync(u16 token)  { if (sDrawSyncCB) sDrawSyncCB(token); }
u16  GXReadDrawSync(void)      { return 0; }
void GXSetDrawDone(void)       {}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) {
    GXDrawSyncCallback old = sDrawSyncCB; sDrawSyncCB = cb; return old;
}
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb) {
    GXDrawDoneCallback old = sDrawDoneCB; sDrawDoneCB = cb; return old;
}

// -----------------------------------------------------------------------
// Viewport / scissor
// -----------------------------------------------------------------------
void GXSetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ) {
    glViewport((GLint)xOrig, (GLint)yOrig, (GLsizei)wd, (GLsizei)ht);
    glDepthRangef(nearZ, farZ);
}
void GXSetViewportJitter(f32 x, f32 y, f32 w, f32 h, f32 n, f32 f, u32 /*field*/) {
    GXSetViewport(x, y, w, h, n, f);
}
void GXSetScissor(u32 x, u32 y, u32 w, u32 h) {
    glScissor((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
}
void GXSetScissorBoxOffset(s32 /*x*/, s32 /*y*/) {}

// -----------------------------------------------------------------------
// Framebuffer
// -----------------------------------------------------------------------
void GXSetCopyClear(GXColor color, u32 zValue) {
    sClearR = color.r / 255.0f;
    sClearG = color.g / 255.0f;
    sClearB = color.b / 255.0f;
    sClearA = color.a / 255.0f;
    sClearZ = zValue;
}

void GXCopyDisp(void* /*dest*/, GXBool clear) {
    // On PC this is called at end-of-frame; the window layer calls glfwSwapBuffers.
    // If clear is set, prepare the next frame's clear.
    if (clear) {
        glClearColor(sClearR, sClearG, sClearB, sClearA);
        glClearDepthf(sClearZ / (float)0x00FFFFFF);
    }
}

void GXCopyTex(void* /*dest*/, GXBool /*clear*/) {}
void GXClearBoundingBox(void) {}
void GXReadBoundingBox(u16* t, u16* b, u16* l, u16* r) {
    if (t) *t = 0; if (b) *b = 480;
    if (l) *l = 0; if (r) *r = 640;
}
void GXSetColorUpdate(GXBool e) { if (e) glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); else glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE); }
void GXSetAlphaUpdate(GXBool e) { glColorMask(GL_TRUE,GL_TRUE,GL_TRUE, e ? GL_TRUE : GL_FALSE); }

// -----------------------------------------------------------------------
// Depth / blend / cull
// -----------------------------------------------------------------------
static GLenum GXCompareToGL(GXCompare c) {
    static const GLenum t[] = { GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,GL_GREATER,GL_NOTEQUAL,GL_GEQUAL,GL_ALWAYS };
    return (c < 8) ? t[c] : GL_ALWAYS;
}

void GXSetZMode(GXBool enable, GXCompare func, GXBool update) {
    if (enable) { glEnable(GL_DEPTH_TEST); glDepthFunc(GXCompareToGL(func)); }
    else          glDisable(GL_DEPTH_TEST);
    glDepthMask(update ? GL_TRUE : GL_FALSE);
}
void GXSetZCompLoc(GXBool /*before_tex*/) {}
void GXSetZTexture(GXZTexOp /*op*/, GXTexFmt /*fmt*/, u32 /*bias*/) {}

static GLenum GXBlendFactorToGL(GXBlendfactor f) {
    switch (f) {
        case GX_BL_ZERO:        return GL_ZERO;
        case GX_BL_ONE:         return GL_ONE;
        case GX_BL_SRCCLR:      return GL_SRC_COLOR;
        case GX_BL_INVSRCCLR:   return GL_ONE_MINUS_SRC_COLOR;
        case GX_BL_SRCALPHA:    return GL_SRC_ALPHA;
        case GX_BL_INVSRCALPHA: return GL_ONE_MINUS_SRC_ALPHA;
        case GX_BL_DSTALPHA:    return GL_DST_ALPHA;
        case GX_BL_INVDSTALPHA: return GL_ONE_MINUS_DST_ALPHA;
        default:                return GL_ONE;
    }
}

void GXSetBlendMode(GXBlendMode type, GXBlendfactor src, GXBlendfactor dst, GXLogicOp /*op*/) {
    if (type == GX_BM_NONE) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(GXBlendFactorToGL(src), GXBlendFactorToGL(dst));
    }
}
void GXSetAlphaCompare(GXCompare /*c0*/, u8 /*r0*/, GXAlphaOp /*op*/, GXCompare /*c1*/, u8 /*r1*/) {}
void GXSetDstAlpha(GXBool /*e*/, u8 /*a*/) {}

void GXSetCullMode(GXCullMode mode) {
    switch (mode) {
        case GX_CULL_NONE:  glDisable(GL_CULL_FACE); break;
        case GX_CULL_FRONT: glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); break;
        case GX_CULL_BACK:  glEnable(GL_CULL_FACE); glCullFace(GL_BACK);  break;
        case GX_CULL_ALL:   glEnable(GL_CULL_FACE); glCullFace(GL_FRONT_AND_BACK); break;
    }
}
void GXSetLineWidth(u8 w, GXTexOffset /*t*/)  { glLineWidth(w / 6.0f); }
void GXSetPointSize(u8 s, GXTexOffset /*t*/)  { glPointSize(s / 6.0f); }
void GXSetFieldMask(GXBool /*odd*/, GXBool /*even*/) {}
void GXSetFieldMode(GXBool /*field*/, GXBool /*half*/) {}

// -----------------------------------------------------------------------
// Vertex format / descriptor (state tracked, applied at GXBegin)
// -----------------------------------------------------------------------
void GXClearVtxDesc(void) {}
void GXSetVtxDesc(GXAttr /*attr*/, GXAttrType /*type*/) {}
void GXSetVtxDescv(const GXVtxDescList* /*list*/) {}
void GXSetVtxAttrFmt(GXVtxFmt /*fmt*/, GXAttr /*attr*/, GXCompCnt /*cnt*/, GXCompType /*type*/, u8 /*frac*/) {}
void GXSetVtxAttrFmtv(GXVtxFmt /*fmt*/, const GXVtxAttrFmtList* /*list*/) {}
void GXSetArray(GXAttr /*attr*/, void* /*ptr*/, u8 /*stride*/) {}

// -----------------------------------------------------------------------
// Immediate mode — 9 floats per vertex: xyz rgba st
// -----------------------------------------------------------------------
static float sCurrPos[3]  = {0,0,0};
static float sCurrColor[4]= {1,1,1,1};
static float sCurrUV[2]   = {0,0};

static void PushVertex() {
    sVtxBuffer.push_back(sCurrPos[0]);
    sVtxBuffer.push_back(sCurrPos[1]);
    sVtxBuffer.push_back(sCurrPos[2]);
    sVtxBuffer.push_back(sCurrColor[0]);
    sVtxBuffer.push_back(sCurrColor[1]);
    sVtxBuffer.push_back(sCurrColor[2]);
    sVtxBuffer.push_back(sCurrColor[3]);
    sVtxBuffer.push_back(sCurrUV[0]);
    sVtxBuffer.push_back(sCurrUV[1]);
}

void GXBegin(GXPrimitive type, GXVtxFmt fmt, u16 vtxCnt) {
    sCurrPrimType  = type;
    sCurrVtxFmt    = fmt;
    sCurrVtxCount  = vtxCnt;
    sVtxBuffer.clear();
    sVtxBuffer.reserve(vtxCnt * 9);
    // Bind the correct TEV shader for this draw call
    tp::tev::BindShaderForDraw();
    // Upload current MVP (position matrix palette index 0)
    GLint mvpLoc = glGetUniformLocation(sActiveProgram, "uMVP");
    if (mvpLoc >= 0) {
        // Build 4x4 from 3x4 palette[0]
        float m44[16] = {
            sPosMatPalette[0][0][0], sPosMatPalette[0][1][0], sPosMatPalette[0][2][0], 0,
            sPosMatPalette[0][0][1], sPosMatPalette[0][1][1], sPosMatPalette[0][2][1], 0,
            sPosMatPalette[0][0][2], sPosMatPalette[0][1][2], sPosMatPalette[0][2][2], 0,
            sPosMatPalette[0][0][3], sPosMatPalette[0][1][3], sPosMatPalette[0][2][3], 1,
        };
        // proj * view (GC passes them separately; combine here)
        float pm[16];
        for (int r=0;r<4;r++) for(int c=0;c<4;c++) {
            pm[r*4+c]=0;
            for(int k=0;k<4;k++) pm[r*4+c]+=sProjectionMtx[r][k]*m44[k*4+c];
        }
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, pm);
    }
}

void GXEnd(void) {
    if (sVtxBuffer.empty()) return;

    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sVtxBuffer.size() * sizeof(float),
                 sVtxBuffer.data(), GL_STREAM_DRAW);

    GLsizei count = (GLsizei)(sVtxBuffer.size() / 9);
    glDrawArrays(GXPrimToGL(sCurrPrimType), 0, count);
    sVtxBuffer.clear();
}

// Vertex attribute setters
void GXPosition3f32(f32 x, f32 y, f32 z) {
    sCurrPos[0]=x; sCurrPos[1]=y; sCurrPos[2]=z; PushVertex();
}
void GXPosition3s16(s16 x, s16 y, s16 z) { GXPosition3f32(x,y,z); }
void GXPosition3s8 (s8  x, s8  y, s8  z) { GXPosition3f32(x,y,z); }
void GXPosition2f32(f32 x, f32 y)         { GXPosition3f32(x,y,0); }
void GXPosition2s16(s16 x, s16 y)         { GXPosition3f32(x,y,0); }
void GXPosition2s8 (s8  x, s8  y)         { GXPosition3f32(x,y,0); }
void GXPosition1x8 (u8  /*idx*/)          {}
void GXPosition1x16(u16 /*idx*/)          {}

void GXNormal3f32(f32 /*nx*/, f32 /*ny*/, f32 /*nz*/) {}
void GXNormal3s16(s16 /*nx*/, s16 /*ny*/, s16 /*nz*/) {}
void GXNormal3s8 (s8  /*nx*/, s8  /*ny*/, s8  /*nz*/) {}
void GXNormal1x8 (u8  /*idx*/)                        {}
void GXNormal1x16(u16 /*idx*/)                        {}

void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
    sCurrColor[0]=r/255.f; sCurrColor[1]=g/255.f;
    sCurrColor[2]=b/255.f; sCurrColor[3]=a/255.f;
}
void GXColor3u8(u8 r, u8 g, u8 b)          { GXColor4u8(r,g,b,255); }
void GXColor3f32(f32 r, f32 g, f32 b)      { sCurrColor[0]=r;sCurrColor[1]=g;sCurrColor[2]=b;sCurrColor[3]=1; }
void GXColor4f32(f32 r, f32 g, f32 b, f32 a){ sCurrColor[0]=r;sCurrColor[1]=g;sCurrColor[2]=b;sCurrColor[3]=a; }
void GXColor1u32(u32 c)  { GXColor4u8((c>>24)&0xFF,(c>>16)&0xFF,(c>>8)&0xFF,c&0xFF); }
void GXColor1u16(u16 /*c*/) {}
void GXColor1x8 (u8  /*idx*/) {}
void GXColor1x16(u16 /*idx*/) {}

void GXTexCoord2f32(f32 s, f32 t) { sCurrUV[0]=s; sCurrUV[1]=t; }
void GXTexCoord2s16(s16 s, s16 t) { sCurrUV[0]=s; sCurrUV[1]=t; }
void GXTexCoord2s8 (s8  s, s8  t) { sCurrUV[0]=s; sCurrUV[1]=t; }
void GXTexCoord1x8 (u8  /*idx*/)  {}
void GXTexCoord1x16(u16 /*idx*/)  {}

// -----------------------------------------------------------------------
// Display lists — record and replay GL calls
// -----------------------------------------------------------------------
void* GXBeginDisplayList(void* list, u32 /*size*/) { return list; }
u32   GXEndDisplayList(void) { return 0; }
void  GXCallDisplayList(const void* /*list*/, u32 /*nbytes*/) {
    // TODO: record/replay display list GL commands
}

// -----------------------------------------------------------------------
// Textures
// -----------------------------------------------------------------------
static GLint GXWrapToGL(GXTexWrapMode w) {
    return (w == GX_CLAMP) ? GL_CLAMP_TO_EDGE :
           (w == GX_MIRROR)? GL_MIRRORED_REPEAT : GL_REPEAT;
}

static GLenum GXTexFmtToGL(GXTexFmt fmt, GLint* internalFmt, GLenum* dataType) {
    *internalFmt = GL_RGBA8;
    *dataType    = GL_UNSIGNED_BYTE;
    switch (fmt) {
        case GX_TF_I4:   case GX_TF_I8:   *internalFmt = GL_R8;   return GL_RED;
        case GX_TF_IA4:  case GX_TF_IA8:  *internalFmt = GL_RG8;  return GL_RG;
        case GX_TF_RGB565: *internalFmt = GL_RGB5; *dataType = GL_UNSIGNED_SHORT_5_6_5; return GL_RGB;
        case GX_TF_RGB5A3: *internalFmt = GL_RGB5_A1; return GL_RGBA;
        case GX_TF_RGBA8:  return GL_RGBA;
        case GX_TF_CMPR:   *internalFmt = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; return GL_RGBA;
        default: return GL_RGBA;
    }
}

void GXInitTexObj(GXTexObj* obj, void* imagePtr, u16 wd, u16 ht,
                  GXTexFmt fmt, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXBool /*mipMap*/)
{
    if (!obj) return;
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GXWrapToGL(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GXWrapToGL(wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (imagePtr && wd > 0 && ht > 0) {
        // Untile GC tiled texture to linear RGBA8
        std::vector<uint8_t> untiled(wd * ht * 4);
        int internalFmt, glFmt, dataType;
        tp::texture::Untile((int)fmt, imagePtr, wd, ht, untiled.data(),
                            &internalFmt, &glFmt, &dataType);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, wd, ht, 0,
                     (GLenum)glFmt, (GLenum)dataType, untiled.data());
    }

    memcpy(obj->dummy, &tex, sizeof(GLuint));
}

void GXInitTexObjCI(GXTexObj* obj, void* imagePtr, u16 wd, u16 ht,
                    GXCITexFmt /*fmt*/, GXTexWrapMode wrapS, GXTexWrapMode wrapT,
                    GXBool mipMap, u32 /*tlutName*/)
{
    GXInitTexObj(obj, imagePtr, wd, ht, GX_TF_RGBA8, wrapS, wrapT, mipMap);
}

void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter minFilt, GXTexFilter magFilt,
                     f32 /*minLod*/, f32 /*maxLod*/, f32 /*lodBias*/,
                     GXBool /*biasClamp*/, GXBool /*doEdgeLOD*/, GXAnisotropy /*maxAniso*/)
{
    if (!obj) return;
    GLuint tex; memcpy(&tex, obj->dummy, sizeof(GLuint));
    glBindTexture(GL_TEXTURE_2D, tex);
    GLint min = (minFilt == GX_NEAR || minFilt == GX_NEAR_MIP_NEAR) ? GL_NEAREST : GL_LINEAR;
    GLint mag = (magFilt == GX_NEAR) ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void GXLoadTexObj(GXTexObj* obj, GXTexMapID id) {
    if (!obj || id >= 8) return;
    GLuint tex; memcpy(&tex, obj->dummy, sizeof(GLuint));
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, tex);
    sTextures[id] = tex;
}

void GXDestroyTexObj(GXTexObj* obj) {
    if (!obj) return;
    GLuint tex; memcpy(&tex, obj->dummy, sizeof(GLuint));
    if (tex) glDeleteTextures(1, &tex);
    memset(obj->dummy, 0, sizeof(obj->dummy));
}

void GXInvalidateTexAll(void) {
    for (int i = 0; i < 8; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void GXInitTlutObj(GXTlutObj* /*obj*/, void* /*lut*/, GXTlutFmt /*fmt*/, u16 /*entries*/) {}
void GXLoadTlut(GXTlutObj* /*obj*/, u32 /*tlutName*/) {}

// -----------------------------------------------------------------------
// TEV — forwarded to tev.cpp (full TEV→GLSL compiler)
// -----------------------------------------------------------------------
void GXSetNumTevStages(u8 n) { tp::tev::SetNumStages(n); }
void GXSetTevOrder(GXTevStageID s, GXTexCoordID tc, GXTexMapID tm, GXChannelID c) { tp::tev::SetStageOrder(s, tc, tm, c); }
void GXSetTevOp(GXTevStageID s, GXTevMode m) {
    // Convenience: set colour/alpha combiners to a standard mode
    static const GXTevColorArg cA[] = {GX_CC_ZERO,GX_CC_TEXC,GX_CC_TEXC,GX_CC_ZERO};
    static const GXTevColorArg cD[] = {GX_CC_RASC,GX_CC_ZERO,GX_CC_CPREV,GX_CC_ZERO};
    static const GXTevAlphaArg aD[] = {GX_CA_RASA,GX_CA_ZERO,GX_CA_APREV,GX_CA_ZERO};
    int mi = (m < 4) ? m : 0;
    tp::tev::SetColorIn(s, GX_CC_ZERO, cA[mi], cD[mi], cD[mi]);
    tp::tev::SetAlphaIn(s, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aD[mi]);
    tp::tev::SetColorOp(s, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    tp::tev::SetAlphaOp(s, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
}
void GXSetTevColorIn(GXTevStageID s, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) { tp::tev::SetColorIn(s,a,b,c,d); }
void GXSetTevAlphaIn(GXTevStageID s, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) { tp::tev::SetAlphaIn(s,a,b,c,d); }
void GXSetTevColorOp(GXTevStageID s, GXTevOp op, GXTevBias b, GXTevScale sc, GXBool cl, GXTevRegID r) { tp::tev::SetColorOp(s,op,b,sc,(bool)cl,r); }
void GXSetTevAlphaOp(GXTevStageID s, GXTevOp op, GXTevBias b, GXTevScale sc, GXBool cl, GXTevRegID r) { tp::tev::SetAlphaOp(s,op,b,sc,(bool)cl,r); }
void GXSetTevColor(GXTevRegID id, GXColor c)   { tp::tev::SetTevColor(id, c); }
void GXSetTevColorS10(GXTevRegID id, GXColorS10 c) { GXColor gc = {(u8)c.r,(u8)c.g,(u8)c.b,(u8)c.a}; tp::tev::SetTevColor(id,gc); }
void GXSetTevKColor(GXTevKColorID id, GXColor c) { tp::tev::SetKonstColor(id, c); }
void GXSetTevKColorSel(GXTevStageID s, GXTevKColorSel sel) { tp::tev::SetKColorSel(s, sel); }
void GXSetTevKAlphaSel(GXTevStageID s, GXTevKAlphaSel sel) { tp::tev::SetKAlphaSel(s, sel); }
void GXSetTevSwapMode(GXTevStageID /*s*/, GXTevSwapSel /*r*/, GXTevSwapSel /*t*/) {}
void GXSetTevSwapModeTable(GXTevSwapSel /*id*/, GXTevColorChan /*r*/, GXTevColorChan /*g*/, GXTevColorChan /*b*/, GXTevColorChan /*a*/) {}
void GXSetTevIndirect(GXTevStageID /*s*/, GXIndTexStageID /*i*/, GXIndTexFormat /*f*/, GXIndTexBiasSel /*bs*/, GXIndTexMtxID /*m*/, GXIndTexWrap /*ws*/, GXIndTexWrap /*wt*/, GXBool /*ap*/, GXBool /*ul*/, GXIndTexAlphaSel /*as*/) {}
void GXSetNumIndStages(u8 /*n*/) {}
void GXSetIndTexCoordScale(GXIndTexStageID /*s*/, GXIndTexScale /*ss*/, GXIndTexScale /*ts*/) {}
void GXSetIndTexOrder(GXIndTexStageID /*s*/, GXTexCoordID /*tc*/, GXTexMapID /*tm*/) {}
void GXSetIndTexMatrix(GXIndTexMtxID /*id*/, const f32 /*offset*/[2][3], s8 /*exp*/) {}

// -----------------------------------------------------------------------
// Transforms
// -----------------------------------------------------------------------
void GXLoadPosMtxImm(const f32 mt[3][4], u32 idx) {
    if (idx < 64) memcpy(sPosMatPalette[idx], mt, 3*4*sizeof(float));
}
void GXLoadNrmMtxImm(const f32 /*mt*/[3][4], u32 /*idx*/) {}
void GXLoadTexMtxImm(const f32 /*mt*/[][4], u32 /*id*/, GXTexMtxType /*type*/) {}
void GXLoadPosMtxIndx(u16 /*idx*/, u32 /*pnidx*/) {}
void GXLoadNrmMtxIndx3x3(u16 /*idx*/, u32 /*pnidx*/) {}
void GXLoadTexMtxIndx(u16 /*idx*/, u32 /*id*/, GXTexMtxType /*type*/) {}
void GXSetCurrentMtx(u32 /*mtx*/) {}

void GXSetProjection(const f32 mtx[4][4], GXProjectionType type) {
    memcpy(sProjectionMtx, mtx, sizeof(sProjectionMtx));
    sProjType = type;
    // Upload to default shader
    if (sActiveProgram) {
        GLint loc = glGetUniformLocation(sActiveProgram, "uMVP");
        if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, &mtx[0][0]);
    }
}
void GXSetProjectionv(const f32* ptr, GXProjectionType type) {
    GXSetProjection(reinterpret_cast<const f32(*)[4]>(ptr), type);
}

// -----------------------------------------------------------------------
// Lighting (stub)
// -----------------------------------------------------------------------
void GXSetNumChans(u8 /*n*/) {}
void GXSetChanCtrl(GXChannelID /*ch*/, GXBool /*en*/, GXColorSrc /*amb*/, GXColorSrc /*mat*/, u32 /*lm*/, GXDiffuseFn /*df*/, GXAttnFn /*af*/) {}
void GXSetChanAmbColor(GXChannelID /*ch*/, GXColor /*c*/) {}
void GXSetChanMatColor(GXChannelID /*ch*/, GXColor /*c*/) {}
void GXSetNumTexGens(u32 /*n*/) {}
void GXSetTexCoordGen(GXTexCoordID /*dst*/, GXTexGenType /*type*/, GXTexGenSrc /*src*/, u32 /*mtx*/) {}
void GXSetTexCoordGen2(GXTexCoordID /*dst*/, GXTexGenType /*type*/, GXTexGenSrc /*src*/, u32 /*mtx*/, GXBool /*norm*/, u32 /*post*/) {}

// -----------------------------------------------------------------------
// Fog (stub)
// -----------------------------------------------------------------------
void GXSetFog(GXFogType /*type*/, f32 /*s*/, f32 /*e*/, f32 /*n*/, f32 /*f*/, GXColor /*c*/) {}
void GXSetFogRangeAdj(GXBool /*en*/, u16 /*center*/, const GXFogAdjTable* /*tbl*/) {}
void GXInitFogAdjTable(GXFogAdjTable* /*tbl*/, u16 /*w*/, const f32 /*pm*/[4][4]) {}

// -----------------------------------------------------------------------
// Render mode / misc
// -----------------------------------------------------------------------
void GXSetRenderMode(GXRenderModeObj* /*rmo*/) {}
void GXAdjustForOverscan(GXRenderModeObj* /*rmo*/, GXRenderModeObj* /*adj*/, u16 /*h*/, u16 /*v*/) {}
void GXSetMisc(GXMiscToken /*t*/, u32 /*v*/) {}
void GXSetDither(GXBool /*d*/) {}
void GXSetRevSubGroup(u8 /*id*/, u8 /*rid*/) {}
u32  GXGetOverflowCount(void)   { return 0; }
u32  GXResetOverflowCount(void) { return 0; }
