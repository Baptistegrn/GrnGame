#pragma once

#include "grngame/utils/c_cpp.h"

BEGIN_DECLARATIONS
#include "../math/types.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL_render.h>

#define PALETTE_COLOR_FILE "scripts/PaletteColor.txt"

static const SDL_Color default_colors[] = {
    // Grayscale
    {0, 0, 0, 255},
    {255, 255, 255, 255},
    {128, 128, 128, 255},
    {192, 192, 192, 255},
    // Reds
    {255, 0, 0, 255},
    {139, 0, 0, 255},
    {220, 20, 60, 255},
    {250, 128, 114, 255},
    // Orange / Yellow
    {255, 165, 0, 255},
    {255, 215, 0, 255},
    {255, 255, 0, 255},
    {245, 245, 220, 255},
    // Greens
    {0, 255, 0, 255},
    {0, 100, 0, 255},
    {50, 205, 50, 255},
    {128, 128, 0, 255},
    // Blues
    {0, 255, 255, 255},
    {0, 128, 128, 255},
    {0, 0, 255, 255},
    {0, 0, 128, 255},
    // Light blues
    {135, 206, 235, 255},
    {64, 224, 208, 255},
    {65, 105, 225, 255},
    {75, 0, 130, 255},
    // Purple / Pink
    {128, 0, 128, 255},
    {238, 130, 238, 255},
    {255, 0, 255, 255},
    {255, 192, 203, 255},
    // Browns
    {139, 69, 19, 255},
    {210, 105, 30, 255},
    {210, 180, 140, 255},
    {111, 78, 55, 255}};

void InitPalettesArrays();
void LoadAllPalettes();
void RemoveAllPalettes();

END_DECLARATIONS