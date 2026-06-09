#pragma once

#include "../assets/asset_manager.h"
#include "../math/types.h"
#include "grngame/utils/c_cpp.h"

BEGIN_DECLARATIONS

typedef struct
{
    uint16 w;
    uint16 h;
    Texture *tex; // for optimisation
    const char *name;
} Sprite;

bool SpriteDraw(Sprite s, uint16 frame, float x, float y, uint8 c, int16 r, uint8 a);

END_DECLARATIONS