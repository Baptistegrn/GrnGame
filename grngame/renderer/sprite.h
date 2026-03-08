#pragma once

#include "../assets/asset_manager.h"
#include "../math/types.h"

typedef struct
{
    uint16 w;
    uint16 h;
    Texture *tex; // for optimisation
    const char *name;
} Sprite;

bool SpriteDraw(Sprite s, uint16 frame, float x, float y, uint8 c, bool f, int16 r, uint8 a);