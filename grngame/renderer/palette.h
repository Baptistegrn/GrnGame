#pragma once

#include "grngame/utils/c_cpp.h"

BEGIN_DECLARATIONS

#include <SDL3/SDL_render.h>

void PaletteInit();
void PaletteDestroy();
void PaletteAddColor(SDL_Color color);
SDL_Color ColorFromHex(const char *hex);
void HexPaletteHashInit();
void PaletteReload();
void PaletteParse();

END_DECLARATIONS