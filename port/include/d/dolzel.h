#pragma once
/**
 * port/include/d/dolzel.h
 * PC port shadow of d/dolzel.h (precompiled-header gateway).
 *
 * The original dolzel.pch pulls in J3D, Z2Audio, JParticle and other
 * GC-only subsystems that are not yet ported.  This stub includes only
 * the headers that are safe under a standard C++17 compiler plus the
 * port abstraction layer.
 */
#ifndef DOLZEL_H
#define DOLZEL_H

// Standard C++ headers (safe everywhere)
#include <cmath>
#include <cstring>
#include <limits>

// Compiler/Metrowerks compatibility macros must be visible before heavy
// decomp headers such as J3DTransform.h.
#include "port/compat.h"

// Port types baseline
#include "port/types.h"

// Core JSystem utilities — all have port stubs
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JHostIO/JORMContext.h"

// SSystem geometry & math — source already enabled
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "SSystem/SComponent/c_m3d_g_aab.h"
#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "SSystem/SComponent/c_bg_s_chk.h"
#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "SSystem/SComponent/c_bg_s_gnd_chk.h"
#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "SSystem/SComponent/c_phase.h"

#ifndef ASSERT
#define ASSERT(expr) ((void)0)
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(x) ((x) * (static_cast<f32>(M_PI) / 180.0f))
#endif

// m_Do helpers — port stubs exist
#include "m_Do/m_Do_hostIO.h"
#ifndef TP_PORT_SKIP_M_DO_EXT
#include "m_Do/m_Do_ext.h"
#endif

// Core framework actor type — defines fopAc_ac_c used throughout game code
#include "f_op/f_op_actor.h"

// Game stubs — port stubs exist
#include "d/d_particle.h"
#include "d/d_particle_name.h"
#include "d/d_com_inf_game.h"
#include "d/d_stage.h"
#include "d/d_kankyo.h"
#if TP_PORT_USE_REAL_ALINK
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_w_base.h"
#include "Z2AudioLib/Z2LinkMgr.h"
#include "d/actor/d_a_spinner.h"
#endif
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_alink.h"
#include "m_Do/m_Do_mtx.h"

// J3D model and animation types — port stubs with minimal class bodies
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J2DGraph/J2DAnimation.h"
// J3D view matrix system (provides j3dSys global)
#include "JSystem/J3DGraphBase/J3DSys.h"

// Forward declarations for other heavy GC subsystems
class J3DAnmColor;
class J3DAnmTransformKey;
class J3DMaterial;
class J3DTexture;
struct J3DPacket;
class J2DScreen;
class J2DAnmTextureSRTKey;

// JPABaseEmitter — particle emitter stub; setRate() used by fopAcM_effHamonSet
#if !TP_PORT_USE_REAL_ALINK
class JPABaseEmitter {
public:
    void setRate(f32) {}
    void setParticleCallBackID(u8) {}
    void setGlobalScale2D(f32, f32) {}
};
class JPAParticle;
#endif

class Z2SoundStarter;
namespace Z2Calc {
    struct FNoise1f;
}

#endif /* DOLZEL_H */
