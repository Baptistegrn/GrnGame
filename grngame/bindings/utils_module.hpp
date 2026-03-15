#pragma once
#include <daScript/daScript.h>

extern "C"
{
#include <cglm/types-struct.h>
}

MAKE_TYPE_FACTORY(vec2s, vec2s)
MAKE_TYPE_FACTORY(ivec2s, ivec2s)

class UtilsModule : public das::Module
{
  public:
    UtilsModule();
};