#pragma once
/**
 * port/include/d/d_stage.h
 * PC port stub — stage/scene data types.
 * Shadows the original which pulls in d_kankyo.h, f_op_actor_mng.h → dolzel.h chain.
 */
#ifndef D_D_STAGE_H
#define D_D_STAGE_H

#include "port/types.h"

#define dStage_NAME_LENGTH 8

enum StageType {
    ST_FIELD      = 0,
    ST_DUNGEON    = 1,
    ST_ROOM       = 2,
    ST_BOSS_ROOM  = 3,
    ST_CASTLE_TOWN = 4,
    ST_CLOUD_SEA  = 5,
};

struct dStage_nodeHeader {
    u32 m_tag;
    int m_entryNum;
    u32 m_offset;
};

struct dStage_startStage_dt {};

// Minimal FileList stub — just needs to exist for pointer declarations
class dStage_FileList_dt_c {
public:
    u32 mField_0x0;
    u8  mEnemyAppear1;
    u8  mBitSw;
    char _pad[2];
};

// Minimal room data stub with getFileListInfo()
struct dStage_roomDt_c {
    dStage_FileList_dt_c* mFileListInfo = nullptr;
    dStage_FileList_dt_c* getFileListInfo() const { return mFileListInfo; }
};

// Stage room control stub
struct dStage_roomControl_c {
    static void onNoArcBank() {}
    dStage_roomDt_c* getStatusRoomDt(int /*room_no*/) { return nullptr; }
    static int getProcID() { return -1; }
};

// Camera/arrow data stubs — referenced by d_camera.h (dCamMapToolData members)
struct stage_camera2_data_class {
    char m_cam_type[16];
    u8   m_arrow_idx;
    u8   field_0x11;
    u8   field_0x12;
    u8   field_0x13;
    u16  field_0x14;
    u16  field_0x16;
};  // Size: 0x18

struct stage_arrow_data_class {
    f32  posX, posY, posZ;
    s16  angleX, angleY, angleZ;
    s16  field_0x12;
};  // Size: 0x14

// Map event stub — referenced by d_camera.h (dCamera_event_data::field_0xec)
struct dStage_MapEvent_dt_c {
    u8 type; u8 field_0x1; u8 field_0x2; u8 field_0x3;
    u8 field_0x4; u8 field_0x5; u8 priority; u8 field_0x7;
    u8 field_0x8; u8 field_0x9; u8 field_0xA; u8 field_0xB;
    u8 field_0xC;
    union {
        char event_name[13];
        struct {
            u8 field_0xd[0x14 - 0xD];
            u16 field_0x14;
            u8 field_0x16; u8 field_0x17;
            u8 sound_type; u8 field_0x19;
        } maptool;
    } data;
    u8 field_0x1a;
    u8 switch_no;
};

struct dStage_MapEventInfo_c {
    int num;
    dStage_MapEvent_dt_c* m_entries;
};

inline int dStage_MapEvent_dt_c_getEventSCutSW(dStage_MapEvent_dt_c*) { return 0; }
inline int dStage_MapEvent_dt_c_getEventSCutType(dStage_MapEvent_dt_c*) { return 0; }

// FileList accessor functions (all return safe defaults — no data on PC)
inline u32 dStage_FileList_dt_GetEnemyAppear1Flag(dStage_FileList_dt_c* /*p*/) { return 0; }
inline u8  dStage_FileList_dt_GetBitSw(dStage_FileList_dt_c* /*p*/)            { return 0xFF; }
inline f32 dStage_FileList_dt_SeaLevel(dStage_FileList_dt_c* /*p*/)            { return 0.0f; }
inline u8  dStage_FileList_dt_GetDefaultCamera(dStage_FileList_dt_c* /*p*/)    { return 0; }
inline int dStage_FileList_dt_GlobalWindLevel(dStage_FileList_dt_c* /*p*/)     { return 0; }
inline int dStage_FileList_dt_GlobalWindDir(dStage_FileList_dt_c* /*p*/)       { return 0; }
inline u32 dStage_FileList_dt_getMiniMap(dStage_FileList_dt_c* /*p*/)          { return 0; }

#endif /* D_D_STAGE_H */
