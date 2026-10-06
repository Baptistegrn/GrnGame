#include "sound.h"
#include "filter.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/math/math.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <math.h>
#include <string.h>

#define SOUND_STOP_FADE_MS 80
#define SOUND_PITCH_MIN 0.01f
#define SOUND_PITCH_MAX 100.0f
#define SOUND_DEPTH 1.0f

typedef void (*SlotAction)(SoundSlot *slot);

static void SlotRefreshPosition(SoundSlot *slot);

static SoundManager *Manager(void)
{
    return &g_app.sound_manager;
}

static bool HasPosition(vec2s position)
{
    return !isnan(position.x) && !isnan(position.y);
}

static bool SamePosition(vec2s a, vec2s b)
{
    return fabsf(a.x - b.x) < SOUND_POSITION_EPSILON && fabsf(a.y - b.y) < SOUND_POSITION_EPSILON;
}

static bool SlotIsActive(const SoundSlot *slot)
{
    return slot->track && MIX_TrackPlaying(slot->track);
}

static bool SlotIsFree(const SoundSlot *slot)
{
    return !slot->track || (!MIX_TrackPlaying(slot->track) && !MIX_TrackPaused(slot->track));
}

static bool SlotMatches(const SoundSlot *slot, const char *name)
{
    return SlotIsActive(slot) && strcmp(slot->name, name) == 0;
}

static void ForEachSlot(SlotAction action)
{
    SoundManager *manager = Manager();
    for (int32 i = 0; i < SOUND_MAX_SLOTS; i++)
        action(&manager->slots[i]);
}

static void ForEachNamed(const char *name, SlotAction action)
{
    SoundManager *manager = Manager();
    for (int32 i = 0; i < SOUND_MAX_SLOTS; i++)
        if (SlotMatches(&manager->slots[i], name))
            action(&manager->slots[i]);
}

static SoundSlot *AcquireSlot(void)
{
    SoundManager *manager = Manager();
    for (int32 i = 0; i < SOUND_MAX_SLOTS; i++)
        if (SlotIsFree(&manager->slots[i]))
            return &manager->slots[i];
    return NULL;
}

static bool SlotEnsureTrack(SoundSlot *slot)
{
    if (!slot->track)
        slot->track = MIX_CreateTrack(Manager()->mixer);
    return slot->track != NULL;
}

static void SDLCALL FilterCallback(void *userdata, MIX_Track *track, const SDL_AudioSpec *spec, float *pcm, int samples)
{
    FilterChainProcess((FilterChain *)userdata, pcm, samples, spec->channels);
}

static void SlotClearFilters(SoundSlot *slot)
{
    MIX_SetTrackCookedCallback(slot->track, NULL, NULL);
    FilterChainDestroy(slot->filters);
    slot->filters = NULL;
}

static bool SlotApplyFilters(SoundSlot *slot, const SoundInfo *info)
{
    SlotClearFilters(slot);
    if (info->filter_count <= 0)
        return true;

    SDL_AudioSpec spec;
    if (!MIX_GetMixerFormat(Manager()->mixer, &spec))
        return false;

    slot->filters = FilterChainCreate(info->filters, info->filter_count, &spec);
    return slot->filters && MIX_SetTrackCookedCallback(slot->track, FilterCallback, slot->filters);
}

static bool SlotApplyPan(SoundSlot *slot, float32 pan)
{
    if (pan == 0.0f)
        return MIX_SetTrack3DPosition(slot->track, NULL);

    pan = CLAMP(pan, -1.0f, 1.0f);
    MIX_StereoGains gains = {1.0f - max(pan, 0.0f), 1.0f + min(pan, 0.0f)};
    return MIX_SetTrackStereo(slot->track, &gains);
}

static MIX_Point3D ToSoundSpace(vec2s world)
{
    SoundManager *manager = Manager();
    vec2s listener = manager->listener;
    float32 half_width = max(manager->half_view.x, 1.0f);
    float32 half_height = max(manager->half_view.y, 1.0f);

    MIX_Point3D point = {(world.x - listener.x) / half_width, (world.y - listener.y) / half_height, -SOUND_DEPTH};
    return point;
}

void SetSoundViewSize(float32 width, float32 height)
{
    Manager()->half_view = (vec2s){.x = width * 0.5f, .y = height * 0.5f};
    ForEachSlot(SlotRefreshPosition);
}

static bool SlotApplyPosition(SoundSlot *slot)
{
    MIX_Point3D point = ToSoundSpace(slot->position);
    return MIX_SetTrack3DPosition(slot->track, &point);
}

static bool SlotApplySpatial(SoundSlot *slot, const SoundInfo *info)
{
    slot->position = info->position;
    return HasPosition(slot->position) ? SlotApplyPosition(slot) : SlotApplyPan(slot, info->pan);
}

static void SlotRefreshPosition(SoundSlot *slot)
{
    if (SlotIsActive(slot) && HasPosition(slot->position))
        SlotApplyPosition(slot);
}

static bool SlotConfigure(SoundSlot *slot, MIX_Audio *audio, const char *name, const SoundInfo *info)
{
    SDL_strlcpy(slot->name, name, sizeof(slot->name));
    return MIX_SetTrackAudio(slot->track, audio) && MIX_SetTrackGain(slot->track, info->volume) &&
           MIX_SetTrackFrequencyRatio(slot->track, CLAMP(info->pitch, SOUND_PITCH_MIN, SOUND_PITCH_MAX)) &&
           SlotApplySpatial(slot, info) && SlotApplyFilters(slot, info);
}

static SDL_PropertiesID PlayOptions(const SoundInfo *info)
{
    SDL_PropertiesID options = SDL_CreateProperties();
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, info->looping ? -1 : 0);
    if (info->fade_in > 0.0f)
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, (Sint64)(info->fade_in * 1000.0f));
    return options;
}

static bool SlotStart(SoundSlot *slot, const SoundInfo *info)
{
    SDL_PropertiesID options = PlayOptions(info);
    bool ok = MIX_PlayTrack(slot->track, options);
    SDL_DestroyProperties(options);
    return ok;
}

static void SlotFadeOut(SoundSlot *slot)
{
    MIX_StopTrack(slot->track, MIX_TrackMSToFrames(slot->track, SOUND_STOP_FADE_MS));
}

static void SlotCut(SoundSlot *slot)
{
    MIX_StopTrack(slot->track, 0);
}

static void SlotDestroy(SoundSlot *slot)
{
    if (!slot->track)
        return;
    SlotClearFilters(slot);
    MIX_DestroyTrack(slot->track);
    slot->track = NULL;
}

bool SoundPlay(const char *name, const SoundInfo *info)
{
    SoundInfo fallback = SoundInfoDefault();
    if (!info)
        info = &fallback;

    MIX_Audio *audio = FindAudio(name);
    if (!audio)
    {
        LOG_WARNING("Sound not found: %s", name);
        return false;
    }

    SoundSlot *slot = AcquireSlot();
    if (!slot || !SlotEnsureTrack(slot))
        return false;

    return SlotConfigure(slot, audio, name, info) && SlotStart(slot, info);
}

void SoundStop(const char *name)
{
    ForEachNamed(name, SlotFadeOut);
}

void SoundBreak(const char *name)
{
    ForEachNamed(name, SlotCut);
}

bool SoundIsPlaying(const char *name)
{
    SoundManager *manager = Manager();
    for (int32 i = 0; i < SOUND_MAX_SLOTS; i++)
        if (SlotMatches(&manager->slots[i], name))
            return true;
    return false;
}

bool SoundIsPlayingAt(const char *name, float32 x, float32 y)
{
    SoundManager *manager = Manager();
    vec2s position = {.x = x, .y = y};
    for (int32 i = 0; i < SOUND_MAX_SLOTS; i++)
        if (SlotMatches(&manager->slots[i], name) && SamePosition(manager->slots[i].position, position))
            return true;
    return false;
}

void SetListenerPosition(float32 x, float32 y)
{
    Manager()->listener = (vec2s){.x = x, .y = y};
    ForEachSlot(SlotRefreshPosition);
}

void SoundShutdown(void)
{
    ForEachSlot(SlotDestroy);
}