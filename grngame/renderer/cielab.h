#pragma once

#include "SDL3/SDL_pixels.h"
#include "grngame/math/types.h"

typedef struct
{
    double x, y, z;
} ColorXYZ;

typedef struct
{
    double l, a, b;
} ColorLAB;

void InitLinearLut();
ColorLAB RgbToLab(SDL_Color *color);
int32 FindBestPaletteColorCIEDE2000(ColorLAB *target_pixel);