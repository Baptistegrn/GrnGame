#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <SDL3/SDL_audio.h>

BEGIN_DECLARATIONS

typedef enum
{
    FILTER_ECHO,
    FILTER_BASSBOOST
} FilterType;

typedef struct
{
    FilterType type;
    union {
        struct
        {
            float32 delay, decay, wet;
        } echo;
        struct
        {
            float32 boost;
        } bassboost;
    };
} FilterDef;

typedef struct FilterChain FilterChain;

FilterDef FilterEcho(float32 delay, float32 decay, float32 wet);
FilterDef FilterBassboost(float32 boost);

FilterChain *FilterChainCreate(const FilterDef *defs, int32 count, const SDL_AudioSpec *spec);
void FilterChainDestroy(FilterChain *chain);
void FilterChainProcess(FilterChain *chain, float32 *pcm, int32 samples, int32 channels);

END_DECLARATIONS