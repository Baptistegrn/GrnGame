#include "sound.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/string_compat.h"
#include <cglm/types-struct.h>
#include <khash.h>
#include <kvec.h>
#include <math.h>

typedef struct
{
    unsigned int handle;
    const char *name;
    vec2s position;
} SFXInstance;

// can contain same name sound more than 1 time
static kvec_t(SFXInstance) s_active_sfx;

typedef struct
{
    void *ptr;
    void (*destroy)(void *);
} FilterHandle;

typedef struct
{
    kvec_t(FilterHandle) active_filters;
    unsigned int handle;
    bool playing;
    vec2s position;
    float volume;
} MusicState;

KHASH_MAP_INIT_STR(MusicStateMap, MusicState)
static khash_t(MusicStateMap) *s_music_states = NULL;

static vec2s s_listener_pos = {{0.f, 0.f}};
static float32 s_max_distance = 1000.f;

static MusicState *GetOrCreateMusicState(const char *name);
static void ClearFilters(MusicState *state, WavStream *stream);
static void ApplyFilters(MusicState *state, WavStream *stream, const SoundInfo *info);
static bool IsPositional(vec2s pos);

static bool IsPositional(vec2s pos)
{
    return !isnan(pos.x) && !isnan(pos.y);
}

static bool PositionMatch(vec2s a, float x, float y)
{
    float dx = a.x - x;
    float dy = a.y - y;
    return (dx * dx + dy * dy) < (SOUND_POSITION_EPSILON * SOUND_POSITION_EPSILON);
}

static void Apply3dSource(Soloud *soloud, unsigned int handle, float x, float y)
{
    Soloud_set3dSourceMinMaxDistance(soloud, handle, 1.f, s_max_distance);
    Soloud_set3dSourceAttenuation(soloud, handle, 1 /* LINEAR_DISTANCE */, 1.f);
    Soloud_set3dSourcePosition(soloud, handle, x, y, 0.f);
}

COLD void SoundInit()
{
#ifndef WASM
    kv_init(s_active_sfx);
    s_music_states = kh_init(MusicStateMap);
    float w = (float)g_app.info.window_universe_width;
    float h = (float)g_app.info.window_universe_height;
    s_max_distance = sqrtf(w * w + h * h);
#endif
}

bool SoundPlaySFX(const char *name, const SoundInfo *info)
{
#ifndef WASM
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    Soloud *soloud = g_app.sound_manager.soloud;

    khiter_t k = kh_get(SoundMap, sound_map, name);
    if (k == kh_end(sound_map))
    {
        LOG_WARNING("Failed to play SFX: '%s' not found", name);
        return false;
    }

    WavStream *stream = kh_value(sound_map, k);
    bool positional = IsPositional(info->position);

    unsigned int handle;
    if (positional)
    {
        handle = Soloud_play3dEx(soloud, stream, info->position.x, info->position.y, 0.f, 0.f, 0.f, 0.f, info->volume,
                                 1 /* paused */, 0);
        Apply3dSource(soloud, handle, info->position.x, info->position.y);
        Soloud_update3dAudio(soloud);
        Soloud_setRelativePlaySpeed(soloud, handle, info->pitch);
        Soloud_setPan(soloud, handle, info->pan);
        Soloud_setLooping(soloud, handle, info->looping);
        if (info->fade_in > 0.f)
            Soloud_fadeVolume(soloud, handle, info->volume, info->fade_in);
        else
            Soloud_setVolume(soloud, handle, info->volume);
        Soloud_setPause(soloud, handle, 0);
    }
    else
    {
        handle = Soloud_play(soloud, stream);
        Soloud_setVolume(soloud, handle, info->volume);
        Soloud_setRelativePlaySpeed(soloud, handle, info->pitch);
        Soloud_setPan(soloud, handle, info->pan);
        Soloud_setLooping(soloud, handle, info->looping);
        if (info->fade_in > 0.f)
            Soloud_fadeVolume(soloud, handle, info->volume, info->fade_in);
    }

    SFXInstance instance = {
        .handle = handle,
        .name = name,
        .position = positional ? info->position : (vec2s){{NAN, NAN}},
    };
    kv_push(SFXInstance, s_active_sfx, instance);

#endif
    return true;
}

bool SFXIsPlaying(const char *name)
{
#ifndef WASM
    Soloud *soloud = g_app.sound_manager.soloud;
    for (int i = 0; i < (int)kv_size(s_active_sfx); i++)
    {
        SFXInstance *inst = &kv_A(s_active_sfx, i);
        if (strcmp(inst->name, name) == 0 && Soloud_isValidVoiceHandle(soloud, inst->handle))
            return true;
    }
#endif
    return false;
}

bool SFXIsPlayingAt(const char *name, float x, float y)
{
#ifndef WASM
    Soloud *soloud = g_app.sound_manager.soloud;
    for (int i = 0; i < (int)kv_size(s_active_sfx); i++)
    {
        SFXInstance *inst = &kv_A(s_active_sfx, i);
        if (strcmp(inst->name, name) == 0 && Soloud_isValidVoiceHandle(soloud, inst->handle) &&
            PositionMatch(inst->position, x, y))
            return true;
    }
#endif
    return false;
}

void SoundUpdate()
{
#ifndef WASM
    Soloud *soloud = g_app.sound_manager.soloud;
    for (int i = 0; i < (int)kv_size(s_active_sfx); i++)
    {
        if (!Soloud_isValidVoiceHandle(soloud, kv_A(s_active_sfx, i).handle))
        {
            kv_A(s_active_sfx, i) = kv_A(s_active_sfx, kv_size(s_active_sfx) - 1);
            kv_size(s_active_sfx)--;
            i--;
        }
    }
#endif
}

static MusicState *GetOrCreateMusicState(const char *name)
{
    if (!s_music_states)
        s_music_states = kh_init(MusicStateMap);

    khiter_t k = kh_get(MusicStateMap, s_music_states, name);
    if (k != kh_end(s_music_states))
        return &kh_value(s_music_states, k);

    MusicState state = {0};
    kv_init(state.active_filters);
    int ret;
    k = kh_put(MusicStateMap, s_music_states, strdup(name), &ret);
    kh_value(s_music_states, k) = state;
    return &kh_value(s_music_states, k);
}

static void ClearFilters(MusicState *state, WavStream *stream)
{
#ifndef WASM
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
#endif
}

static void ApplyFilters(MusicState *state, WavStream *stream, const SoundInfo *info)
{
#ifndef WASM
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
#endif
}

bool SoundPlayMusic(const char *name, const SoundInfo *info)
{
#ifndef WASM
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    Soloud *soloud = g_app.sound_manager.soloud;

    khiter_t k = kh_get(SoundMap, sound_map, name);
    if (k == kh_end(sound_map))
    {
        LOG_WARNING("Failed to play music: '%s' not found", name);
        return false;
    }

    WavStream *stream = kh_value(sound_map, k);
    MusicState *state = GetOrCreateMusicState(name);

    if (state->playing)
        MusicStop(name);

    ApplyFilters(state, stream, info);

    bool positional = IsPositional(info->position);
    unsigned int handle;
    if (positional)
    {
        handle = Soloud_play3dEx(soloud, stream, info->position.x, info->position.y, 0.f, 0.f, 0.f, 0.f, info->volume,
                                 1 /* paused */, 0);
        Apply3dSource(soloud, handle, info->position.x, info->position.y);
        Soloud_update3dAudio(soloud);
        Soloud_setRelativePlaySpeed(soloud, handle, info->pitch);
        Soloud_setPan(soloud, handle, info->pan);
        Soloud_setLooping(soloud, handle, info->looping);
        if (info->fade_in > 0.f)
            Soloud_fadeVolume(soloud, handle, info->volume, info->fade_in);
        else
            Soloud_setVolume(soloud, handle, info->volume);
        Soloud_setPause(soloud, handle, 0);
        state->position = info->position;
    }
    else
    {
        handle = Soloud_play(soloud, stream);
        Soloud_setVolume(soloud, handle, info->volume);
        Soloud_setRelativePlaySpeed(soloud, handle, info->pitch);
        Soloud_setPan(soloud, handle, info->pan);
        Soloud_setLooping(soloud, handle, info->looping);
        if (info->fade_in > 0.f)
            Soloud_fadeVolume(soloud, handle, info->volume, info->fade_in);
        state->position = (vec2s){{NAN, NAN}};
    }

    state->handle = handle;
    state->volume = info->volume;
    state->playing = true;

#endif
    return true;
}

void MusicStop(const char *name)
{
#ifndef WASM
    khiter_t k = kh_get(MusicStateMap, s_music_states, name);
    if (k == kh_end(s_music_states))
        return;

    MusicState *state = &kh_value(s_music_states, k);
    Soloud_stop(g_app.sound_manager.soloud, state->handle);
    state->playing = false;
#endif
}

bool MusicIsPlaying(const char *name)
{
#ifndef WASM
    khiter_t k = kh_get(MusicStateMap, s_music_states, name);
    if (k == kh_end(s_music_states))
        return false;

    MusicState *state = &kh_value(s_music_states, k);
    state->playing = Soloud_isValidVoiceHandle(g_app.sound_manager.soloud, state->handle);
    return state->playing;
#else
    (void)name;
    return false;
#endif
}

bool MusicIsPlayingAt(const char *name, float x, float y)
{
#ifndef WASM
    khiter_t k = kh_get(MusicStateMap, s_music_states, name);
    if (k == kh_end(s_music_states))
        return false;

    MusicState *state = &kh_value(s_music_states, k);
    state->playing = Soloud_isValidVoiceHandle(g_app.sound_manager.soloud, state->handle);
    if (!state->playing)
        return false;

    return PositionMatch(state->position, x, y);
#else
    (void)name;
    (void)x;
    (void)y;
    return false;
#endif
}

void SetListenerPosition(float32 x, float32 y)
{
#ifndef WASM
    s_listener_pos.x = x;
    s_listener_pos.y = y;
    Soloud_set3dListenerPosition(g_app.sound_manager.soloud, x, y, 0.f);
    Soloud_update3dAudio(g_app.sound_manager.soloud);
#endif
}