#pragma once

#include "../assets/asset_manager.h"
#include "../math/types.h"
#include "cglm/types-struct.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

Texture *TextureGet(const char *name);
bool TextureDraw(const char *name, float x, float y, uint8 coefficient, int16 rotation, uint8 alpha);
ivec2s TextureGetSize(const char *name);

END_DECLARATIONS