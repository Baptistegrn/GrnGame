#include "filter.h"

FilterDef FilterReverb(float room, float damp, float width, float wet)
{
    return (FilterDef){.type = FILTER_REVERB, .reverb = {room, damp, width, wet}};
}

FilterDef FilterEcho(float delay, float decay, float wet)
{
    return (FilterDef){.type = FILTER_ECHO, .echo = {delay, decay, wet}};
}

FilterDef FilterBassboost(float boost)
{
    return (FilterDef){.type = FILTER_BASSBOOST, .bassboost = {boost}};
}