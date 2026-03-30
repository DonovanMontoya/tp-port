/**
 * f_pc_draw.cpp
 * Framework - Process Draw
 */

#include "f_pc/f_pc_draw.h"
#include "SSystem/SComponent/c_API_graphic.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_pause.h"
#include "port/port.h"

static u32 sPortDrawFrame = 0;
static u32 sPortDrawCallsThisFrame = 0;

int fpcDw_Execute(base_process_class* i_proc) {
    if (i_proc == NULL) {
        return 0;
    }

    if (!fpcPause_IsEnable(i_proc, 2)) {
        layer_class* save_layer;
        layer_class* draw_layer;
        int ret;
        process_method_func draw_func;
    
        save_layer = fpcLy_CurrentLayer();
        if (fpcBs_Is_JustOfType(g_fpcLf_type, i_proc->subtype)) {
            draw_func = ((leafdraw_method_class*)i_proc->methods)->draw_method;
        } else {
            draw_func = ((nodedraw_method_class*)i_proc->methods)->draw_method;
        }
    
        draw_layer = i_proc->layer_tag.layer;
        if (draw_layer == NULL) {
            draw_layer = save_layer != NULL ? save_layer : fpcLy_RootLayer();
            i_proc->layer_tag.layer = draw_layer;
        }

        fpcLy_SetCurrentLayer(draw_layer);
        ret = draw_func(i_proc);
        fpcLy_SetCurrentLayer(save_layer);
        if (sPortDrawFrame <= 120 && ret != 0) {
            ++sPortDrawCallsThisFrame;
        }
        return ret;
    }

    return 0;
}

int fpcDw_Handler(fpcDw_HandlerFuncFunc i_iterHandler, fpcDw_HandlerFunc i_func) {
    int ret;
    ++sPortDrawFrame;
    if (sPortDrawFrame <= 120) {
        sPortDrawCallsThisFrame = 0;
    }
    cAPIGph_BeforeOfDraw();
    ret = i_iterHandler(i_func);
    cAPIGph_AfterOfDraw();
    if (sPortDrawFrame <= 120) {
        tp::log::info("fpcDw_Handler: frame=%u draw_calls=%u ret=%d", sPortDrawFrame,
                      sPortDrawCallsThisFrame, ret);
    }
    return ret;
}
