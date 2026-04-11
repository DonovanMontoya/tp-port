/**
 * f_pc_executor.cpp
 * Framework - Process Executor
 */

#include "f_pc/f_pc_executor.h"
#include "f_pc/f_pc_debug_sv.h"
#include "f_pc/f_pc_manager.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_pause.h"
#include "f_pc/f_pc_searcher.h"

base_process_class* fpcEx_Search(fpcLyIt_JudgeFunc i_judgeFunc, void* i_data) {
    return (base_process_class*)fpcLyIt_AllJudge(i_judgeFunc, i_data);
}

base_process_class* fpcEx_SearchByID(fpc_ProcID i_id) {
    if (!(i_id == fpcM_UNK_PROCESS_ID_e || i_id == fpcM_ERROR_PROCESS_ID_e)) {
        return fpcEx_Search(fpcSch_JudgeByID, &i_id);
    }
    return NULL;
}

BOOL fpcEx_IsExist(fpc_ProcID i_id) {
    if (fpcEx_SearchByID(i_id) != NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int fpcEx_Execute(base_process_class* i_proc) {
    if (i_proc == NULL) {
        return 0;
    }

    if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
        i_proc->profname == fpcNm_PLAY_SCENE_e) {
        tp::log::info("fpcEx_Execute: %s proc=%p init_state=%d pause=%d",
                      i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                      : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                   : "PLAY_SCENE",
                      i_proc, i_proc->state.init_state, fpcPause_IsEnable(i_proc, 1));
    }
    if (i_proc->state.init_state != 2 || fpcPause_IsEnable(i_proc, 1) == TRUE)
        return 0;

    return fpcBs_Execute(i_proc);
}

int fpcEx_ToLineQ(base_process_class* i_proc) {
    if (i_proc == NULL) {
        return 0;
    }

    if (i_proc->layer_tag.layer == NULL) {
        i_proc->layer_tag.layer = fpcLy_CurrentLayer();
        if (i_proc->layer_tag.layer == NULL) {
            i_proc->layer_tag.layer = fpcLy_RootLayer();
        }
        if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
            i_proc->profname == fpcNm_PLAY_SCENE_e) {
            tp::log::info("fpcEx_ToLineQ: %s repaired null layer -> %p",
                          i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                          : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                       : "PLAY_SCENE",
                          i_proc->layer_tag.layer);
        }
    }

    if (i_proc->layer_tag.layer == NULL) {
        if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
            i_proc->profname == fpcNm_PLAY_SCENE_e) {
            tp::log::info("fpcEx_ToLineQ: %s missing layer/process layer=%p process_node=%p",
                          i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                          : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                       : "PLAY_SCENE",
                          i_proc->layer_tag.layer,
                          NULL);
        }
        return 0;
    }

    base_process_class* process =
        i_proc->layer_tag.layer->process_node != NULL ? &i_proc->layer_tag.layer->process_node->base
                                                      : NULL;

    if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
        i_proc->profname == fpcNm_PLAY_SCENE_e) {
        tp::log::info("fpcEx_ToLineQ: %s proc=%p layer=%d list=%d line_in_use=%d",
                      i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                      : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                   : "PLAY_SCENE",
                      i_proc,
                      i_proc->layer_tag.layer ? i_proc->layer_tag.layer->layer_id : -1,
                      i_proc->priority.current_info.list_id,
                      process != NULL ? cTg_IsUse(&process->line_tag_.base) : 0);
    }

    if (i_proc->layer_tag.layer->layer_id == fpcLy_ROOT_e ||
        (process != NULL && cTg_IsUse(&process->line_tag_.base) == TRUE))
    {
        int var_r28 = i_proc->priority.current_info.list_id;
        if (fpcLnTg_ToQueue(&i_proc->line_tag_, var_r28) == 0) {
            if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
                i_proc->profname == fpcNm_PLAY_SCENE_e) {
                tp::log::info("fpcEx_ToLineQ: %s line queue failed list=%d",
                              i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                              : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                           : "PLAY_SCENE",
                              var_r28);
            }
            fpcLyTg_QueueTo(&i_proc->layer_tag);
            return 0;
        }

#if DEBUG
        if (g_fpcDbSv_service[1] != NULL) {
            g_fpcDbSv_service[1](i_proc);
        }
#endif

        i_proc->state.init_state = 2;
        if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
            i_proc->profname == fpcNm_PLAY_SCENE_e) {
            tp::log::info("fpcEx_ToLineQ: %s proc=%p queued init_state=%d",
                          i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                          : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                       : "PLAY_SCENE",
                          i_proc, i_proc->state.init_state);
        }
        if (fpcBs_Is_JustOfType(g_fpcNd_type, i_proc->subtype)) {
            fpcLyIt_OnlyHere(&((process_node_class*)i_proc)->layer, (fpcLyIt_OnlyHereFunc)fpcEx_ToLineQ, i_proc);
        }

        return 1;
    }

    if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
        i_proc->profname == fpcNm_PLAY_SCENE_e) {
        tp::log::info("fpcEx_ToLineQ: %s deferred by parent line tag", 
                      i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                      : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                   : "PLAY_SCENE");
    }

    return 0;
}

int fpcEx_ExecuteQTo(base_process_class* i_proc) {
    if (i_proc == NULL) {
        return 0;
    }

    if (fpcLyTg_QueueTo(&i_proc->layer_tag) == 1) {
        i_proc->state.init_state = 3;
        return 1;
    }
    
    return 0;
}

int fpcEx_ToExecuteQ(base_process_class* i_proc) {
    if (i_proc == NULL) {
        return 0;
    }

    process_priority_class* priority = &i_proc->priority;
    u16 list_priority = priority->current_info.list_priority;
    if (list_priority == fpcPi_CURRENT_e) {
        list_priority = 0;
    }

    if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
        i_proc->profname == fpcNm_PLAY_SCENE_e) {
        tp::log::info("fpcEx_ToExecuteQ: %s proc=%p layer=%d list=%d prio=%d",
                      i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                      : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                   : "PLAY_SCENE",
                      i_proc, priority->current_info.layer_id, priority->current_info.list_id,
                      list_priority);
    }
    if (fpcLyTg_ToQueue(&i_proc->layer_tag, priority->current_info.layer_id,
                        priority->current_info.list_id, list_priority) == 1) {
        fpcEx_ToLineQ(i_proc);
        return 1;
    }

    if (i_proc->profname == fpcNm_TITLE_e || i_proc->profname == fpcNm_OPENING_SCENE_e ||
        i_proc->profname == fpcNm_PLAY_SCENE_e) {
        tp::log::info("fpcEx_ToExecuteQ: %s queue failed", 
                      i_proc->profname == fpcNm_TITLE_e ? "TITLE"
                      : i_proc->profname == fpcNm_OPENING_SCENE_e ? "OPENING_SCENE"
                                                                   : "PLAY_SCENE");
    }
    
    return 0;
}

void fpcEx_Handler(fpcLnIt_QueueFunc i_queueFunc) {
    fpcLnIt_Queue(i_queueFunc);
}
