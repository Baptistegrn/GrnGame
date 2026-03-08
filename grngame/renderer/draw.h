#pragma once

#include "../assets/asset_manager.h"
#include "../math/types.h"
#include <stdbool.h>

bool TextureDraw(const char *name, float x, float y, uint8 coefficient, bool flip, int16 r, uint8 a);

Texture *TextureGet(const char *name);