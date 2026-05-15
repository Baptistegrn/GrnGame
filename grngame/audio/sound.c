#include "sound.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/string_compat.h"
#include <cglm/types-struct.h>
#include <khash.h>
#include <kvec.h>
#include <math.h>

typedef struct
{
    void *ptr;
    void (*destroy)(void *);
} FilterHandle;

typedef struct
{
    WavStream *stream;
    FilterHandle active_filters[MAX_FILTERS];
} SoundEntry;

typedef struct
{
    kvec_t(FilterHandle) active_filters;
    unsigned int handle;
    bool playing;
    vec2s position;
    float volume;
} SoundState;

KHASH_MAP_INIT_STR(SoundStateMap, SoundState)

static khash_t(SoundStateMap) *s_sound_states = NULL;
static vec2s s_listener_pos = {{0.f, 0.f}};
static float32 s_max_distance = 0.f;

// either get the state of a sound or create an empty one
static SoundState *GetOrCreateState(const char *name);
static void ClearFilters(SoundState *state, WavStream *stream);
static void ApplyFilters(SoundState *state, WavStream *stream, const SoundInfo *info);

// soloud doesn't update volume sound
static float GetAttenuatedVolume(float base_volume, vec2s sound_pos);

COLD void SoundInit()
{
    s_sound_states = kh_init(SoundStateMap);
    s_max_distance = sqrtf((float)g_app.info.window_universe_width * g_app.info.window_universe_width +
                           (float)g_app.info.window_universe_height * g_app.info.window_universe_height);
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
    k = kh_put(SoundStateMap, s_sound_states, strdup(name), &ret);
    kh_value(s_sound_states, k) = state;
    return &kh_value(s_sound_states, k);
}
bool SoundPlay(const char *name, const SoundInfo *info)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    Soloud *soloud = g_app.sound_manager.soloud;

    khiter_t k = kh_get(SoundMap, sound_map, name);
    if (k == kh_end(sound_map))
    {
        LOG_WARNING("Failed to play sound: '%s' not loaded or not found in asset manager", name);
        return false;
    }

    WavStream *stream = kh_value(sound_map, k);
    SoundState *state = GetOrCreateState(name);

    if (state->playing)
        SoundStop(name);

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

    if (positional)
    {
        float32 attenuated_volume = GetAttenuatedVolume(info->volume, info->position);
        Soloud_setVolume(soloud, handle, attenuated_volume);
        state->position = info->position;
        state->volume = info->volume;
    }

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

static float32 GetAttenuatedVolume(float32 base_volume, vec2s sound_pos)
{
    float32 dx = sound_pos.x - s_listener_pos.x;
    float32 dy = sound_pos.y - s_listener_pos.y;
    float32 distance = sqrtf(dx * dx + dy * dy);
    float32 attenuated_volume = base_volume * fmaxf(0.f, 1.f - distance / s_max_distance);
    return attenuated_volume;
}

void SetListenerPosition(float32 x, float32 y)
{
    s_listener_pos.x = x;
    s_listener_pos.y = y;
    Soloud_set3dListenerPosition(g_app.sound_manager.soloud, x, y, 0.f);
    Soloud_update3dAudio(g_app.sound_manager.soloud);

    for (khiter_t k = kh_begin(s_sound_states); k != kh_end(s_sound_states); ++k)
    {
        if (!kh_exist(s_sound_states, k))
            continue;

        SoundState *state = &kh_value(s_sound_states, k);
        if (!state->playing)
            continue;
        float32 attenuated_volume = GetAttenuatedVolume(state->volume, state->position);
        Soloud_setVolume(g_app.sound_manager.soloud, state->handle, attenuated_volume);
    }
}