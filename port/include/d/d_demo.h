#pragma once
/**
 * port/include/d/d_demo.h
 * PC port stub — demo/cutscene system types.
 * Shadows the original which pulls in JStudio → JGeometry chain.
 * On PC cutscenes are not played; all stubs are no-ops.
 */
#ifndef D_D_DEMO_H
#define D_D_DEMO_H

#include "port/types.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"

class dDemo_actor_c {
public:
    void setActor(void* /*p*/) {}
};

class dDemo_c {
public:
    static dDemo_actor_c* getActor(u8 /*id*/) { return nullptr; }
    static bool isStatus(u32 /*status*/) { return false; }
    static void reset() {}
    static u32  getFrame() { return 0; }
};

#endif /* D_D_DEMO_H */
