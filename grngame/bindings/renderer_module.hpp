#pragma once
#include "da_script_engine.hpp"

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
