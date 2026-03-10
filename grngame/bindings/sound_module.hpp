#pragma once
#include "da_script_engine.hpp"
#include "grngame/audio/sound_info.h"

MAKE_TYPE_FACTORY(SoundInfo, SoundInfo);

class SoundModule : public das::Module
{
public:
    SoundModule();
};
