#pragma once

#include "../assets/asset_manager.h"
#include "../math/types.h"
#include <stdbool.h>

Texture *TextureGet(const char *name);
bool TextureDraw(const char *name, float x, float y, uint8 coefficient, bool flip, int16 rotation, uint8 alpha);
