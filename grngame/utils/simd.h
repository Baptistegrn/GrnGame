#pragma once
#include "SDL3/SDL_pixels.h"
#include "c_cpp.h"
#include "grngame/math/types.h"

BEGIN_DECLARATIONS;

typedef struct
{
    uint8_t r[256];
    uint8_t g[256];
    uint8_t b[256];
    int32 count;
} PaletteSIMD;

void BuildPaletteSIMD(PaletteSIMD *SDL_RESTRICT out, const SDL_Color *SDL_RESTRICT colors, int32 count);

void RemapImagePalette(uint8_t *SDL_RESTRICT pixels, int32_t w, int32_t h, int32_t pitch,
                       const PaletteSIMD *SDL_RESTRICT pal, const uint8_t alpha_lut[256]);

END_DECLARATIONS;