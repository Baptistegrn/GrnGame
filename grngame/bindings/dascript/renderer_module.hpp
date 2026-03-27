#pragma once
#include "utils_module.hpp" // we need to include it since it has the type factories of vec2s and ivec2s
#include <daScript/daScript.h>

extern "C"
{
#include "grngame/renderer/particule.h"
#include "grngame/renderer/sprite.h"
}

MAKE_TYPE_FACTORY(ParticleEmitter, ParticleEmitter);
MAKE_TYPE_FACTORY(Sprite, Sprite);

class RendererModule : public das::Module
{
  public:
    RendererModule();
};
