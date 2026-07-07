#include "grngame/audio/filter.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
#include "grngame/audio/speech.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <stdbool.h>
#include <string.h>

static void filter_def_new(WrenVM *vm)
{
    FilterDef *f = (FilterDef *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(FilterDef));
    memset(f, 0, sizeof(FilterDef));
}

COLD static void filter_def_init(WrenVM *vm)
{
    (void)vm;
}

static void filter_def_get_type(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->type);
}

static void filter_def_set_type(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->type = (FilterType)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_reverb_room(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.room);
}

static void filter_def_set_reverb_room(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.room = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_reverb_damp(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.damp);
}

static void filter_def_set_reverb_damp(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.damp = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_reverb_width(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.width);
}

static void filter_def_set_reverb_width(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.width = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_reverb_wet(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.wet);
}

static void filter_def_set_reverb_wet(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.wet = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_echo_delay(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.delay);
}

static void filter_def_set_echo_delay(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.delay = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_echo_decay(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.decay);
}

static void filter_def_set_echo_decay(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.decay = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_echo_wet(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.wet);
}

static void filter_def_set_echo_wet(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.wet = (float32)wrenGetSlotDouble(vm, 1);
}

static void filter_def_get_bassboost_boost(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (float64)((FilterDef *)wrenGetSlotForeign(vm, 0))->bassboost.boost);
}

static void filter_def_set_bassboost_boost(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->bassboost.boost = (float32)wrenGetSlotDouble(vm, 1);
}

static FilterDef static_filters[MAX_FILTERS];

static void parse_sound_info(WrenVM *vm, SoundInfo *info)
{
    info->volume = (float32)wrenGetSlotDouble(vm, 2);
    info->pitch = (float32)wrenGetSlotDouble(vm, 3);
    info->pan = (float32)wrenGetSlotDouble(vm, 4);
    info->looping = wrenGetSlotBool(vm, 5);
    info->fade_in = (float32)wrenGetSlotDouble(vm, 6);
    info->position.x = (float32)wrenGetSlotDouble(vm, 7);
    info->position.y = (float32)wrenGetSlotDouble(vm, 8);

    int32 filter_count = 0;
    memset(static_filters, 0, sizeof(static_filters));

    if (wrenGetSlotType(vm, 9) == WREN_TYPE_LIST)
    {
        filter_count = wrenGetListCount(vm, 9);
        if (filter_count > MAX_FILTERS)
            filter_count = MAX_FILTERS;

        for (int32 i = 0; i < filter_count; i++)
        {
            wrenGetListElement(vm, 9, i, 10);
            FilterDef *f = (FilterDef *)wrenGetSlotForeign(vm, 10);
            static_filters[i] = *f;
        }
    }

    info->filters = static_filters;
    info->filter_count = filter_count;
}

static void sound_play_music(WrenVM *vm)
{
    wrenEnsureSlots(vm, 11);
    const char *name = wrenGetSlotString(vm, 1);
    SoundInfo info;
    parse_sound_info(vm, &info);
    wrenSetSlotBool(vm, 0, SoundPlayMusic(name, &info));
}

static void sound_play_sfx(WrenVM *vm)
{
    wrenEnsureSlots(vm, 11);
    const char *name = wrenGetSlotString(vm, 1);
    SoundInfo info;
    parse_sound_info(vm, &info);
    wrenSetSlotBool(vm, 0, SoundPlaySFX(name, &info));
}

static void speech_say(WrenVM *vm)
{
    wrenEnsureSlots(vm, 11);
    const char *name = wrenGetSlotString(vm, 1);
    SoundInfo info;
    parse_sound_info(vm, &info);
    SpeechSay(name, &info);
}

static void music_stop(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    MusicStop(name);
}

static void music_is_playing(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    wrenSetSlotBool(vm, 0, MusicIsPlaying(name));
}

static void music_is_playing_at(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    float32 x = (float32)wrenGetSlotDouble(vm, 2);
    float32 y = (float32)wrenGetSlotDouble(vm, 3);
    wrenSetSlotBool(vm, 0, MusicIsPlayingAt(name, x, y));
}

static void sfx_is_playing(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    wrenSetSlotBool(vm, 0, SFXIsPlaying(name));
}

static void sfx_is_playing_at(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    float32 x = (float32)wrenGetSlotDouble(vm, 2);
    float32 y = (float32)wrenGetSlotDouble(vm, 3);
    wrenSetSlotBool(vm, 0, SFXIsPlayingAt(name, x, y));
}

static void set_listener_position(WrenVM *vm)
{
    float32 x = (float32)wrenGetSlotDouble(vm, 1);
    float32 y = (float32)wrenGetSlotDouble(vm, 2);
    SetListenerPosition(x, y);
}

void RegisterSoundModule()
{
    const char *filter_mod = "std/wren/audio/filter_def";
    const char *filter_cls = "FilterDef";

    RegisterClass(filter_mod, filter_cls, filter_def_new, NULL);

    RegisterMethod(filter_mod, filter_cls, false, "init new()", filter_def_init);
    RegisterMethod(filter_mod, filter_cls, false, "type", filter_def_get_type);
    RegisterMethod(filter_mod, filter_cls, false, "type=(_)", filter_def_set_type);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_room", filter_def_get_reverb_room);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_room=(_)", filter_def_set_reverb_room);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_damp", filter_def_get_reverb_damp);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_damp=(_)", filter_def_set_reverb_damp);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_width", filter_def_get_reverb_width);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_width=(_)", filter_def_set_reverb_width);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_wet", filter_def_get_reverb_wet);
    RegisterMethod(filter_mod, filter_cls, false, "reverb_wet=(_)", filter_def_set_reverb_wet);
    RegisterMethod(filter_mod, filter_cls, false, "echo_delay", filter_def_get_echo_delay);
    RegisterMethod(filter_mod, filter_cls, false, "echo_delay=(_)", filter_def_set_echo_delay);
    RegisterMethod(filter_mod, filter_cls, false, "echo_decay", filter_def_get_echo_decay);
    RegisterMethod(filter_mod, filter_cls, false, "echo_decay=(_)", filter_def_set_echo_decay);
    RegisterMethod(filter_mod, filter_cls, false, "echo_wet", filter_def_get_echo_wet);
    RegisterMethod(filter_mod, filter_cls, false, "echo_wet=(_)", filter_def_set_echo_wet);
    RegisterMethod(filter_mod, filter_cls, false, "bassboost_boost", filter_def_get_bassboost_boost);
    RegisterMethod(filter_mod, filter_cls, false, "bassboost_boost=(_)", filter_def_set_bassboost_boost);

    const char *sound_mod = "std/wren/audio/sound";
    const char *sound_cls = "Sound";

    RegisterMethod(sound_mod, sound_cls, true, "sound_play_music_(_,_,_,_,_,_,_,_,_)", sound_play_music);
    RegisterMethod(sound_mod, sound_cls, true, "music_stop_(_)", music_stop);
    RegisterMethod(sound_mod, sound_cls, true, "music_is_playing_(_)", music_is_playing);
    RegisterMethod(sound_mod, sound_cls, true, "music_is_playing_at_(_,_,_)", music_is_playing_at);
    RegisterMethod(sound_mod, sound_cls, true, "sound_play_sfx_(_,_,_,_,_,_,_,_,_)", sound_play_sfx);
    RegisterMethod(sound_mod, sound_cls, true, "sfx_is_playing_(_)", sfx_is_playing);
    RegisterMethod(sound_mod, sound_cls, true, "sfx_is_playing_at_(_,_,_)", sfx_is_playing_at);
    RegisterMethod(sound_mod, sound_cls, true, "speech_say_(_,_,_,_,_,_,_,_,_)", speech_say);
    RegisterMethod(sound_mod, sound_cls, true, "set_listener_position_(_,_)", set_listener_position);
}