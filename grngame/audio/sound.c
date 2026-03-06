#include "sound.h"
#include "grngame/core/app.h"
#include "grngame/utils/string.h"
#include <khash.h>
#include <kvec.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
    kvec_t(FilterHandle) active_filters;
    unsigned int handle;
    bool playing;
} SoundState;

KHASH_MAP_INIT_STR(SoundStateMap, SoundState)

static khash_t(SoundStateMap) *s_sound_states = NULL;

// either get the state of a sound or create an empty one
static SoundState *GetOrCreateState(const char *name);
static void ClearFilters(SoundState *state, WavStream *stream);
static void ApplyFilters(SoundState *state, WavStream *stream, const SoundInfo *info);

void SoundSystemInit()
{
    s_sound_states = kh_init(SoundStateMap);
}

static SoundState *GetOrCreateState(const char *name)
{
    if (!s_sound_states)
        s_sound_states = kh_init(SoundStateMap);

    khiter_t k = kh_get(SoundStateMap, s_sound_states, name);
    if (k != kh_end(s_sound_states))
        return &kh_value(s_sound_states, k);

    SoundState state = {0};
    kv_init(state.active_filters);
    int ret;
    k = kh_put(SoundStateMap, s_sound_states, Strdup(name), &ret);
    kh_value(s_sound_states, k) = state;
    return &kh_value(s_sound_states, k);
}

bool SoundPlay(const char *name, const SoundInfo *info)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    Soloud *soloud = g_app.sound_manager.soloud;

    khiter_t k = kh_get(SoundMap, sound_map, name);
    if (k == kh_end(sound_map))
        return false;

    WavStream *stream = kh_value(sound_map, k);
    SoundState *state = GetOrCreateState(name);

    ApplyFilters(state, stream, info);

    bool positional = !isnan(info->position.x) && !isnan(info->position.y);

    unsigned int handle;
    if (positional)
        handle = Soloud_play3d(soloud, stream, info->position.x, info->position.y, 0.f);
    else
        handle = Soloud_play(soloud, stream);

    Soloud_setVolume(soloud, handle, info->volume);
    Soloud_setRelativePlaySpeed(soloud, handle, info->pitch);
    Soloud_setPan(soloud, handle, info->pan);
    Soloud_setLooping(soloud, handle, info->looping);

    if (info->fade_in > 0.0f)
        Soloud_fadeVolume(soloud, handle, info->volume, info->fade_in);

    state->handle = handle;
    state->playing = true;

    return true;
}

bool SoundIsPlaying(const char *name)
{
    khiter_t k = kh_get(SoundStateMap, s_sound_states, name);
    if (k == kh_end(s_sound_states))
        return false;

    SoundState *state = &kh_value(s_sound_states, k);
    // sync with state in soloud
    state->playing = Soloud_isValidVoiceHandle(g_app.sound_manager.soloud, state->handle);
    return state->playing;
}

void SoundStop(const char *name)
{
    khiter_t k = kh_get(SoundStateMap, s_sound_states, name);
    if (k == kh_end(s_sound_states))
        return;

    SoundState *state = &kh_value(s_sound_states, k);
    Soloud_stop(g_app.sound_manager.soloud, state->handle);
    state->playing = false;
}

static void ClearFilters(SoundState *state, WavStream *stream)
{
    for (int i = 0; i < (int)kv_size(state->active_filters); i++)
    {
        FilterHandle *fh = &kv_A(state->active_filters, i);
        if (fh->ptr)
        {
            WavStream_setFilter(stream, i, NULL);
            fh->destroy(fh->ptr);
        }
    }
    kv_size(state->active_filters) = 0;
}

static void ApplyFilters(SoundState *state, WavStream *stream, const SoundInfo *info)
{
    ClearFilters(state, stream);

    for (int i = 0; i < info->filter_count && i < MAX_FILTERS; i++)
    {
        const FilterDef *def = &info->filters[i];
        void *filter = NULL;
        void (*destroy)(void *) = NULL;

        switch (def->type)
        {
        case FILTER_REVERB: {
            FreeverbFilter *r = FreeverbFilter_create();
            FreeverbFilter_setParams(r, def->reverb.wet, def->reverb.room, def->reverb.damp, def->reverb.width);
            filter = r;
            destroy = (void (*)(void *))FreeverbFilter_destroy;
            break;
        }
        case FILTER_ECHO: {
            EchoFilter *e = EchoFilter_create();
            EchoFilter_setParamsEx(e, def->echo.delay, def->echo.decay, def->echo.wet);
            filter = e;
            destroy = (void (*)(void *))EchoFilter_destroy;
            break;
        }
        case FILTER_BASSBOOST: {
            BassboostFilter *b = BassboostFilter_create();
            BassboostFilter_setParams(b, def->bassboost.boost);
            filter = b;
            destroy = (void (*)(void *))BassboostFilter_destroy;
            break;
        }
        }

        if (filter)
        {
            WavStream_setFilter(stream, i, filter);
            kv_push(FilterHandle, state->active_filters, ((FilterHandle){filter, destroy}));
        }
    }
}