#pragma once
/**
 * port/include/d/d_stage.h
 * PC port stub — stage/scene data types.
 * Shadows the original which pulls in f_op_actor_mng.h → dolzel.h chain.
 * f_pc_base.cpp includes this but only uses types for static storage.
 */
#ifndef D_D_STAGE_H
#define D_D_STAGE_H

#include "port/types.h"

enum StageType {
    ST_FIELD      = 0,
    ST_DUNGEON    = 1,
    ST_ROOM       = 2,
    ST_BOSS_ROOM  = 3,
    ST_CASTLE_TOWN = 4,
    ST_CLOUD_SEA  = 5,
};

// Forward declarations only — no game-logic needed on PC
struct dStage_roomControl_c {
    static void onNoArcBank() {}
};

struct dStage_nodeHeader {};
struct dStage_startStage_dt {};

#endif /* D_D_STAGE_H */
