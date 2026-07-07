#pragma once

#include "grngame/math/types.h"

typedef enum
{
    FILTER_REVERB,
    FILTER_ECHO,
    FILTER_BASSBOOST
} FilterType;

typedef struct
{
    FilterType type;
    union {
        struct
        {
            float32 room, damp, width, wet;
        } reverb;
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

FilterDef FilterReverb(float32 room, float32 damp, float32 width, float32 wet);
FilterDef FilterEcho(float32 delay, float32 decay, float32 wet);
FilterDef FilterBassboost(float32 boost);