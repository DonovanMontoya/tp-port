#pragma once
/**
 * port/include/d/d_demo.h
 * PC port stub — demo/cutscene system types.
 * Shadows the original which pulls in JStudio → JGeometry chain.
 * On PC cutscenes are not played; all stubs are no-ops.
 */
#ifndef D_D_DEMO_H
#define D_D_DEMO_H

#include "JSystem/JGadget/binary.h"
#include "JSystem/JStudio/JStudio/stb.h"
#include "port/types.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"

using JGadget::binary::TValueIterator_raw;

struct dDemo_prm_data {
    u8 raw[0x10];
};

class dDemo_prm_c {
public:
    dDemo_prm_c() : mData(nullptr) {}
    dDemo_prm_data* getData() { return mData; }

    u32 field_0x0 = 0;
    dDemo_prm_data* mData;
};

class dDemo_actor_c {
public:
    void setActor(void* /*p*/) {}
    bool checkEnable(u16 /*flag*/) { return false; }
    dDemo_prm_c* getPrm() { return nullptr; }
};

class dDemo_c {
public:
    static void create() {}
    static dDemo_actor_c* getActor(u8 /*id*/) { return nullptr; }
    static bool isStatus(u32 /*status*/) { return false; }
    static void reset() {}
    static void update() {}
    static void remove() {}
    static u32  getFrame() { return 0; }
};

#endif /* D_D_DEMO_H */
