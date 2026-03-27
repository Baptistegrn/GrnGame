#include "sound_module.hpp"

#include "grngame/audio/sound.h"
#include "grngame/audio/speech.h"

struct SoundInfoAnnotation : das::ManagedStructureAnnotation<SoundInfo, false>
{
    SoundInfoAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("SoundInfo", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(volume)>("volume");
        addField<DAS_BIND_MANAGED_FIELD(pitch)>("pitch");
        addField<DAS_BIND_MANAGED_FIELD(pan)>("pan");
        addField<DAS_BIND_MANAGED_FIELD(looping)>("looping");
        addField<DAS_BIND_MANAGED_FIELD(fade_in)>("fade_in");
    }
};

static void speech_say(const char *text, const SoundInfo &sound_info)
{
    SpeechSay(text, &sound_info);
}

static void sound_play(const char *sound_name, const SoundInfo &sound_info = SoundInfoDefault())
{
    SoundPlay(sound_name, &sound_info);
}

SoundModule::SoundModule() : Module("grngame_sound")
{
    auto module_library = das::ModuleLibrary(this);
    module_library.addBuiltInModule();
    module_library.addModule(this);

    addAnnotation(das::make_smart<SoundInfoAnnotation>(module_library));

    das::addExtern<DAS_BIND_FUN(SoundInfoDefault), das::SimNode_ExtFuncCallAndCopyOrMove>(
        *this, module_library, "sound_info_default", das::SideEffects::none, "SoundInfoDefault");

    das::addExtern<DAS_BIND_FUN(SoundInfoAt), das::SimNode_ExtFuncCallAndCopyOrMove>(
        *this, module_library, "sound_info_at", das::SideEffects::none, "SoundInfoAt");

    das::addExtern<DAS_BIND_FUN(sound_play)>(*this, module_library, "sound_play", das::SideEffects::modifyExternal,
                                             "sound_play");

    das::addExtern<DAS_BIND_FUN(SoundIsPlaying)>(*this, module_library, "sound_is_playing", das::SideEffects::none,
                                                 "sound_is_playing");

    das::addExtern<DAS_BIND_FUN(SoundStop)>(*this, module_library, "sound_stop", das::SideEffects::modifyExternal,
                                            "sound_stop");

    das::addExtern<DAS_BIND_FUN(speech_say)>(*this, module_library, "speech_say", das::SideEffects::modifyExternal,
                                             "speech_say");
}

REGISTER_MODULE(SoundModule);