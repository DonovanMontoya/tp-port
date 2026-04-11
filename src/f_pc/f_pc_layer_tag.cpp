/**
 * f_pc_layer_tag.cpp
 * Framework - Process Layer Tag
 */

#include "f_pc/f_pc_layer_tag.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_name.h"
#include "port/port.h"

int fpcLyTg_ToQueue(layer_management_tag_class* i_layer_tag, fpc_ProcID i_layerID, u16 i_listID,
                    u16 i_listPriority) {
    base_process_class* process = static_cast<base_process_class*>(i_layer_tag->create_tag.mpTagData);
    const bool trace = process != NULL &&
                       (process->profname == fpcNm_TITLE_e || process->profname == fpcNm_OPENING_SCENE_e);

    if (i_layer_tag->layer == NULL) {
        if (i_layerID == fpcLy_NONE_e) {
            if (trace) {
                tp::log::info("fpcLyTg_ToQueue: %s failed none layer id",
                              process->profname == fpcNm_TITLE_e ? "TITLE" : "OPENING_SCENE");
            }
            return 0;
        }
        i_layer_tag->layer = fpcLy_Layer(i_layerID);
    }

    if (i_layer_tag->layer == NULL) {
        if (trace) {
            tp::log::info("fpcLyTg_ToQueue: %s failed resolve layer=%u",
                          process->profname == fpcNm_TITLE_e ? "TITLE" : "OPENING_SCENE",
                          i_layerID);
        }
        return 0;
    }

    if (i_layerID != fpcLy_NONE_e && i_layerID != fpcLy_CURRENT_e && i_layer_tag->layer->layer_id != i_layerID) {
        i_layer_tag->layer = fpcLy_Layer(i_layerID);
    }

    if (i_layerID == fpcLy_NONE_e || i_layerID == fpcLy_CURRENT_e) {
        int result = fpcLy_ToQueue(i_layer_tag->layer, i_listID, &i_layer_tag->create_tag);
        if (result != 0) {
            i_layer_tag->node_list_id = i_listID;
            i_layer_tag->node_list_priority = result - 1;
            if (trace) {
                tp::log::info("fpcLyTg_ToQueue: %s queued append layer=%u list=%u prio=%u",
                              process->profname == fpcNm_TITLE_e ? "TITLE" : "OPENING_SCENE",
                              i_layer_tag->layer ? i_layer_tag->layer->layer_id : 0xFFFFFFFF,
                              i_listID, i_layer_tag->node_list_priority);
            }
            return 1;
        }
    } else if (fpcLy_IntoQueue(i_layer_tag->layer, i_listID, &i_layer_tag->create_tag,
                                i_listPriority) != 0)
    {
        i_layer_tag->node_list_id = i_listID;
        i_layer_tag->node_list_priority = i_listPriority;
        if (trace) {
            tp::log::info("fpcLyTg_ToQueue: %s queued insert layer=%u list=%u prio=%u",
                          process->profname == fpcNm_TITLE_e ? "TITLE" : "OPENING_SCENE",
                          i_layer_tag->layer ? i_layer_tag->layer->layer_id : 0xFFFFFFFF,
                          i_listID, i_listPriority);
        }
        return 1;
    }

    if (trace) {
        tp::log::info("fpcLyTg_ToQueue: %s insert failed layer=%u list=%u prio=%u node_list_size=%d",
                      process->profname == fpcNm_TITLE_e ? "TITLE" : "OPENING_SCENE",
                      i_layer_tag->layer ? i_layer_tag->layer->layer_id : 0xFFFFFFFF,
                      i_listID, i_listPriority,
                      i_layer_tag->layer && i_layer_tag->layer->node_tree.mpLists
                          ? i_layer_tag->layer->node_tree.mpLists[i_listID].mSize
                          : -1);
    }

    return 0;
}

int fpcLyTg_QueueTo(layer_management_tag_class* i_layer_tag) {
    if (fpcLy_QueueTo(i_layer_tag->layer, &i_layer_tag->create_tag) == 1) {
        i_layer_tag->layer = NULL;
        i_layer_tag->node_list_id = 0xFFFF;
        i_layer_tag->node_list_priority = 0xFFFF;
        return 1;
    }

    return 0;
}

int fpcLyTg_Move(layer_management_tag_class* i_layer_tag, fpc_ProcID i_layerID, u16 i_listID,
                 u16 i_listPriority) {
    layer_class* layer = fpcLy_Layer(i_layerID);
    if (layer == NULL) {
        return 0;
    } else if (fpcLyTg_QueueTo(i_layer_tag) == 1) {
        i_layer_tag->layer = layer;
        return fpcLyTg_ToQueue(i_layer_tag, i_layerID, i_listID, i_listPriority);
    } else {
        return 0;
    }
}

int fpcLyTg_Init(layer_management_tag_class* i_layer_tag, fpc_ProcID i_id, void* i_data) {
    static layer_management_tag_class crear = {
        NULL, NULL, NULL, NULL, 0, NULL, 0xFFFF, 0xFFFF,
    };

    layer_class* layer;
    *i_layer_tag = crear;
    cTg_Create(&i_layer_tag->create_tag, i_data);

    layer = fpcLy_Layer(i_id);
    if (layer != NULL) {
        i_layer_tag->layer = layer;
        return 1;
    }
    
    return 0;
}
