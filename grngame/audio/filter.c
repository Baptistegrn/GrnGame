#include "filter.h"

FilterDef FilterReverb(float32 room, float32 damp, float32 width, float32 wet)
{
    return (FilterDef){.type = FILTER_REVERB, .reverb = {room, damp, width, wet}};
}

FilterDef FilterEcho(float32 delay, float32 decay, float32 wet)
{
    return (FilterDef){.type = FILTER_ECHO, .echo = {delay, decay, wet}};
}

FilterDef FilterBassboost(float32 boost)
{
    return (FilterDef){.type = FILTER_BASSBOOST, .bassboost = {boost}};
}