#pragma once
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <stdbool.h>

BEGIN_DECLARATIONS;

#define MAX_KEYBOARDS 4

typedef struct
{
    bool key_pressed[SDL_SCANCODE_COUNT];
    bool key_just_pressed[SDL_SCANCODE_COUNT];
    bool key_just_released[SDL_SCANCODE_COUNT];
    SDL_KeyboardID id;
} Keyboard;

bool KeyPressed(int32 index, int32 key);
bool KeyJustPressed(int32 index, int32 key);
bool KeyJustReleased(int32 index, int32 key);

END_DECLARATIONS;