#include "speech.h"

#include "grngame/core/app.h"
#include "grngame/core/param.h"

#include <kvec.h>
#include <math.h>

#ifndef GRNGAME_WASM
typedef struct
{
    void *ptr;
    void (*destroy)(void *);
} FilterHandle;

static kvec_t(FilterHandle) s_active_filters;
static bool s_filters_initialized = false;

static void ClearFilters(Speech *speech)
{
    for (int32 i = 0; i < (int32)kv_size(s_active_filters); i++)
    {
        FilterHandle *fh = &kv_A(s_active_filters, i);
        if (fh->ptr)
        {
            Speech_setFilter(speech, i, NULL);
            fh->destroy(fh->ptr);
        }
    }
    kv_size(s_active_filters) = 0;
}

static void ApplyFilters(Speech *speech, const SoundInfo *info)
{
    if (!s_filters_initialized)
    {
        kv_init(s_active_filters);
        s_filters_initialized = true;
    }

    ClearFilters(speech);

    for (int32 i = 0; i < info->filter_count && i < MAX_FILTERS; i++)
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
            Speech_setFilter(speech, i, filter);
            kv_push(FilterHandle, s_active_filters, ((FilterHandle){filter, destroy}));
        }
    }
}
#endif

void SpeechSay(const char *text, const SoundInfo *sound_info)
{
#ifndef GRNGAME_WASM
    Speech *speech = g_app.sound_manager.speech;
    Soloud soloud = g_app.sound_manager.soloud;

    ApplyFilters(speech, sound_info);

    Speech_setText(speech, text);

    uint32 handle;
    if (!isnan(sound_info->position.x) && !isnan(sound_info->position.y))
    {
        handle = Soloud_play3dEx(soloud, speech, sound_info->position.x, sound_info->position.y, 0.0f, 0.0f, 0.0f, 0.0f,
                                 sound_info->volume, 1, 0);
        Soloud_setRelativePlaySpeed(soloud, handle, sound_info->pitch);
        Soloud_setPan(soloud, handle, sound_info->pan);
        Soloud_setLooping(soloud, handle, sound_info->looping);
        if (sound_info->fade_in > 0.f)
            Soloud_fadeVolume(soloud, handle, sound_info->volume, sound_info->fade_in);
        else
            Soloud_setVolume(soloud, handle, sound_info->volume);
        Soloud_setPause(soloud, handle, 0);
    }
    else
    {
        handle = Soloud_playEx(soloud, speech, sound_info->volume, sound_info->pan, 1, 0);
        Soloud_setRelativePlaySpeed(soloud, handle, sound_info->pitch);
        Soloud_setLooping(soloud, handle, sound_info->looping);
        if (sound_info->fade_in > 0.f)
            Soloud_fadeVolume(soloud, handle, sound_info->volume, sound_info->fade_in);
        else
            Soloud_setVolume(soloud, handle, sound_info->volume);
        Soloud_setPause(soloud, handle, 0);
    }
#endif
}