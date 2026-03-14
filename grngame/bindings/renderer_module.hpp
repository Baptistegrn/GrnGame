#pragma once
#include "cglm/types-struct.h"
#include "daScript/ast/ast_typefactory.h"
#include "da_script_engine.hpp"

extern "C"
{
#include "grngame/renderer/particule.h"
#include "grngame/renderer/sprite.h"
}

MAKE_TYPE_FACTORY(ParticleEmitter, ParticleEmitter);
MAKE_TYPE_FACTORY(Sprite, Sprite);
// todo : move in utils bindings
MAKE_TYPE_FACTORY(ivec2s, ivec2s);

class RendererModule : public das::Module
{
  public:
    RendererModule();
};
