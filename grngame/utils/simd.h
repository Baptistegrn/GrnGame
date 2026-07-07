#pragma once
#include "SDL3/SDL_pixels.h"
#include "c_cpp.h"
#include "grngame/math/types.h"

BEGIN_DECLARATIONS;

typedef struct
{
    uint8 r[256];
    uint8 g[256];
    uint8 b[256];
    int32 count;
} PaletteSIMD;

void BuildPaletteSIMD(PaletteSIMD *SDL_RESTRICT out, const SDL_Color *SDL_RESTRICT colors, int32 count);

void RemapImagePalette(uint8 *SDL_RESTRICT pixels, uint16 w, uint16 h, uint16 pitch,
                       const PaletteSIMD *SDL_RESTRICT pal, const uint8 alpha_lut[256]);

END_DECLARATIONS;