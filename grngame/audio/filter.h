#pragma once

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
            float room, damp, width, wet;
        } reverb;
        struct
        {
            float delay, decay, wet;
        } echo;
        struct
        {
            float boost;
        } bassboost;
    };
} FilterDef;

FilterDef FilterReverb(float room, float damp, float width, float wet);
FilterDef FilterEcho(float delay, float decay, float wet);
FilterDef FilterBassboost(float boost);