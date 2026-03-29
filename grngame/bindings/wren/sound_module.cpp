#include "grngame/bindings/wren/sound_module.hpp"
#include "grngame/audio/filter.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
#include "grngame/audio/speech.h"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/utils/attributes.h"
#include <cstdlib>
#include <cstring>

static void filter_def_new(WrenVM *vm)
{
    FilterDef *f = (FilterDef *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(FilterDef));
    memset(f, 0, sizeof(FilterDef));
}
static void filter_def_init(WrenVM *vm)
{
    // Wren has already allocated foreign memory in allocate callback.
    (void)vm;
}

static void filter_def_get_type(WrenVM *vm)
{
    wren_set<int>(vm, 0, (int)((FilterDef *)wrenGetSlotForeign(vm, 0))->type);
}
static void filter_def_set_type(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->type = (FilterType)wren_get<int>(vm, 1);
}

static void filter_def_get_reverb_room(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.room);
}
static void filter_def_set_reverb_room(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.room = wren_get<float>(vm, 1);
}
static void filter_def_get_reverb_damp(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.damp);
}
static void filter_def_set_reverb_damp(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.damp = wren_get<float>(vm, 1);
}
static void filter_def_get_reverb_width(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.width);
}
static void filter_def_set_reverb_width(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.width = wren_get<float>(vm, 1);
}
static void filter_def_get_reverb_wet(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.wet);
}
static void filter_def_set_reverb_wet(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->reverb.wet = wren_get<float>(vm, 1);
}

static void filter_def_get_echo_delay(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.delay);
}
static void filter_def_set_echo_delay(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.delay = wren_get<float>(vm, 1);
}
static void filter_def_get_echo_decay(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.decay);
}
static void filter_def_set_echo_decay(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.decay = wren_get<float>(vm, 1);
}
static void filter_def_get_echo_wet(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.wet);
}
static void filter_def_set_echo_wet(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->echo.wet = wren_get<float>(vm, 1);
}

static void filter_def_get_bassboost_boost(WrenVM *vm)
{
    wren_set(vm, 0, ((FilterDef *)wrenGetSlotForeign(vm, 0))->bassboost.boost);
}
static void filter_def_set_bassboost_boost(WrenVM *vm)
{
    ((FilterDef *)wrenGetSlotForeign(vm, 0))->bassboost.boost = wren_get<float>(vm, 1);
}

static thread_local FilterDef static_filters[MAX_FILTERS];

static void sound_play(WrenVM *vm)
{
    wrenEnsureSlots(vm, 11);

    const char *name = wren_get<const char *>(vm, 1);
    float volume = wren_get<float>(vm, 2);
    float pitch = wren_get<float>(vm, 3);
    float pan = wren_get<float>(vm, 4);
    bool looping = wren_get<bool>(vm, 5);
    float fade_in = wren_get<float>(vm, 6);
    float pos_x = wren_get<float>(vm, 7);
    float pos_y = wren_get<float>(vm, 8);

    int filter_count = 0;
    memset(static_filters, 0, sizeof(static_filters));

    if (LIKELY(wrenGetSlotType(vm, 9) == WREN_TYPE_LIST))
    {
        filter_count = wrenGetListCount(vm, 9);
        if (filter_count > MAX_FILTERS)
            filter_count = MAX_FILTERS;

        for (int i = 0; i < filter_count; i++)
        {
            wrenGetListElement(vm, 9, i, 10);
            FilterDef *f = (FilterDef *)wrenGetSlotForeign(vm, 10);
            static_filters[i] = *f;
        }
    }

    SoundInfo info = {
        .volume = volume,
        .pitch = pitch,
        .pan = pan,
        .looping = looping,
        .fade_in = fade_in,
        .position = {pos_x, pos_y},
        .filters = static_filters,
        .filter_count = filter_count,
    };

    SoundPlay(name, &info);
}

static void speach_say(WrenVM *vm)
{
    wrenEnsureSlots(vm, 11);

    const char *name = wren_get<const char *>(vm, 1);
    float volume = wren_get<float>(vm, 2);
    float pitch = wren_get<float>(vm, 3);
    float pan = wren_get<float>(vm, 4);
    bool looping = wren_get<bool>(vm, 5);
    float fade_in = wren_get<float>(vm, 6);
    float pos_x = wren_get<float>(vm, 7);
    float pos_y = wren_get<float>(vm, 8);

    int filter_count = 0;
    memset(static_filters, 0, sizeof(static_filters));

    if (LIKELY(wrenGetSlotType(vm, 9) == WREN_TYPE_LIST))
    {
        filter_count = wrenGetListCount(vm, 9);
        if (filter_count > MAX_FILTERS)
            filter_count = MAX_FILTERS;

        for (int i = 0; i < filter_count; i++)
        {
            wrenGetListElement(vm, 9, i, 10);
            FilterDef *f = (FilterDef *)wrenGetSlotForeign(vm, 10);
            static_filters[i] = *f;
        }
    }

    SoundInfo info = {
        .volume = volume,
        .pitch = pitch,
        .pan = pan,
        .looping = looping,
        .fade_in = fade_in,
        .position = {pos_x, pos_y},
        .filters = static_filters,
        .filter_count = filter_count,
    };

    SpeechSay(name, &info);
}

static void sound_stop(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    SoundStop(name);
}

static void sound_is_playing(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    wren_set(vm, 0, SoundIsPlaying(name));
}

static void sound_info_at(WrenVM *vm)
{
    float x = wren_get<float>(vm, 1);
    float y = wren_get<float>(vm, 2);
    SoundInfoAt(x, y);
}

WrenForeignClassMethods BindForeignClassCallbackSound(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    if (strcmp(module, "std/wren/sound") == 0 && strcmp(class_name, "FilterDef") == 0)
        return {filter_def_new, nullptr};

    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackSound(WrenVM *vm, const char *module, const char *class_name,
                                                   bool is_static, const char *signature)
{
    (void)vm;
    if (!module || !class_name || !signature)
        return nullptr;

    if (strcmp(module, "std/wren/sound") == 0)
    {
        if (strcmp(class_name, "Sound") == 0 && is_static)
        {
            if (strcmp(signature, "play(_,_,_,_,_,_,_,_,_)") == 0)
                return sound_play;
            if (strcmp(signature, "stop(_)") == 0)
                return sound_stop;
            if (strcmp(signature, "is_playing(_)") == 0)
                return sound_is_playing;
            if (strcmp(signature, "info_at(_,_)") == 0)
                return sound_info_at;
            if (strcmp(signature, "speach_say(_,_,_,_,_,_,_,_,_)") == 0)
                return speach_say;
        }

        if (strcmp(class_name, "FilterDef") == 0 && !is_static)
        {
            if (strcmp(signature, "init new()") == 0)
                return filter_def_init;
            if (strcmp(signature, "type") == 0)
                return filter_def_get_type;
            if (strcmp(signature, "type=(_)") == 0)
                return filter_def_set_type;
            if (strcmp(signature, "reverb_room") == 0)
                return filter_def_get_reverb_room;
            if (strcmp(signature, "reverb_room=(_)") == 0)
                return filter_def_set_reverb_room;
            if (strcmp(signature, "reverb_damp") == 0)
                return filter_def_get_reverb_damp;
            if (strcmp(signature, "reverb_damp=(_)") == 0)
                return filter_def_set_reverb_damp;
            if (strcmp(signature, "reverb_width") == 0)
                return filter_def_get_reverb_width;
            if (strcmp(signature, "reverb_width=(_)") == 0)
                return filter_def_set_reverb_width;
            if (strcmp(signature, "reverb_wet") == 0)
                return filter_def_get_reverb_wet;
            if (strcmp(signature, "reverb_wet=(_)") == 0)
                return filter_def_set_reverb_wet;
            if (strcmp(signature, "echo_delay") == 0)
                return filter_def_get_echo_delay;
            if (strcmp(signature, "echo_delay=(_)") == 0)
                return filter_def_set_echo_delay;
            if (strcmp(signature, "echo_decay") == 0)
                return filter_def_get_echo_decay;
            if (strcmp(signature, "echo_decay=(_)") == 0)
                return filter_def_set_echo_decay;
            if (strcmp(signature, "echo_wet") == 0)
                return filter_def_get_echo_wet;
            if (strcmp(signature, "echo_wet=(_)") == 0)
                return filter_def_set_echo_wet;
            if (strcmp(signature, "bassboost_boost") == 0)
                return filter_def_get_bassboost_boost;
            if (strcmp(signature, "bassboost_boost=(_)") == 0)
                return filter_def_set_bassboost_boost;
        }
    }

    return nullptr;
}