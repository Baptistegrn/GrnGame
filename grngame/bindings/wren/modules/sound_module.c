#include "grngame/audio/filter.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "wren.h"
#include <grngame/utils/string_compat.h>
#include <math.h>
#include <stdbool.h>

static void filter_def_new(WrenVM *vm)
{
    FilterDef *f = (FilterDef *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(FilterDef));
    CLEAR_PTR(f, 0);
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

static bool filter_is_supported(const FilterDef *filter)
{
    return filter->type == FILTER_ECHO || filter->type == FILTER_BASSBOOST;
}

static int32 parse_filters(WrenVM *vm, int32 list_slot, int32 element_slot)
{
    CLEAR(static_filters, 0);
    if (wrenGetSlotType(vm, list_slot) != WREN_TYPE_LIST)
        return 0;

    int32 count = wrenGetListCount(vm, list_slot);
    int32 kept = 0;
    for (int32 i = 0; i < count && kept < MAX_FILTERS; i++)
    {
        wrenGetListElement(vm, list_slot, i, element_slot);
        const FilterDef *filter = (const FilterDef *)wrenGetSlotForeign(vm, element_slot);
        if (filter_is_supported(filter))
            static_filters[kept++] = *filter;
        else
            LOG_WARNING("Unsupported filter type %d ignored", (int)filter->type);
    }
    return kept;
}

static void parse_sound_info(WrenVM *vm, SoundInfo *info)
{
    info->volume = (float32)wrenGetSlotDouble(vm, 2);
    info->pitch = (float32)wrenGetSlotDouble(vm, 3);
    info->pan = (float32)wrenGetSlotDouble(vm, 4);
    info->looping = wrenGetSlotBool(vm, 5);
    info->fade_in = (float32)wrenGetSlotDouble(vm, 6);
    info->position.x = (float32)wrenGetSlotDouble(vm, 7);
    info->position.y = (float32)wrenGetSlotDouble(vm, 8);
    info->filter_count = parse_filters(vm, 9, 10);
    info->filters = static_filters;
}

static void sound_play(WrenVM *vm)
{
    wrenEnsureSlots(vm, 11);
    const char *name = wrenGetSlotString(vm, 1);
    SoundInfo info;
    parse_sound_info(vm, &info);
    wrenSetSlotBool(vm, 0, SoundPlay(name, &info));
}

static void sound_stop(WrenVM *vm)
{
    SoundStop(wrenGetSlotString(vm, 1));
}

static void sound_break(WrenVM *vm)
{
    SoundBreak(wrenGetSlotString(vm, 1));
}

static void sound_is_playing(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, SoundIsPlaying(wrenGetSlotString(vm, 1)));
}

static void sound_is_playing_at(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    float32 x = (float32)wrenGetSlotDouble(vm, 2);
    float32 y = (float32)wrenGetSlotDouble(vm, 3);
    wrenSetSlotBool(vm, 0, SoundIsPlayingAt(name, x, y));
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

    RegisterClass_(filter_mod, filter_cls, filter_def_new, NULL);

    RegisterMethod(filter_mod, filter_cls, false, "init new()", filter_def_init);
    RegisterMethod(filter_mod, filter_cls, false, "type", filter_def_get_type);
    RegisterMethod(filter_mod, filter_cls, false, "type=(_)", filter_def_set_type);
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

    RegisterMethod(sound_mod, sound_cls, true, "sound_play_(_,_,_,_,_,_,_,_,_)", sound_play);
    RegisterMethod(sound_mod, sound_cls, true, "sound_stop_(_)", sound_stop);
    RegisterMethod(sound_mod, sound_cls, true, "sound_break_(_)", sound_break);
    RegisterMethod(sound_mod, sound_cls, true, "sound_is_playing_(_)", sound_is_playing);
    RegisterMethod(sound_mod, sound_cls, true, "sound_is_playing_at_(_,_,_)", sound_is_playing_at);
    RegisterMethod(sound_mod, sound_cls, true, "set_listener_position_(_,_)", set_listener_position);
}