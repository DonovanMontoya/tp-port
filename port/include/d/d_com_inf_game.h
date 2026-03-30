#pragma once

// Use the real game info surface on this fast-port branch, then layer any
// PC-only helpers on top as needed.
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "d/d_bg_s.h"
#include_next "d/d_com_inf_game.h"
