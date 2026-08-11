#pragma once

#include "cielab.h"
#include "grngame/data/json.h"
#include "grngame/utils/c_cpp.h"
#include "kvec.h"
#include <SDL3/SDL_pixels.h>
#include <khash.h>

BEGIN_DECLARATIONS

KHASH_MAP_INIT_STR(ColorHex, SDL_Color);

typedef struct
{
    kvec_t(SDL_Color) palette_elements;
    kvec_t(ColorLAB) palette_elements_lab;
    khash_t(ColorHex) * palette_hex_hash;
} PaletteManager;

PaletteManager PaletteManagerCreate(void);
void PaletteManagerDestroy(PaletteManager *manager);
void PaletteFreeStringVec(string_vec_t *vec);

void PaletteSetFromConfig(void);
void PaletteAddColor(SDL_Color color);
SDL_Color ColorFromHex(const char *hex);
void PaletteParse(string_vec_t *text);
void PaletteReload(void);

void PaletteRead();

END_DECLARATIONS
