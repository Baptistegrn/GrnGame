#pragma once

#include "controller.h"
#include "input_text.h"
#include "mouse.h"
#include <SDL3/SDL_scancode.h>
#include <kvec.h>
#include <stdbool.h>

typedef struct
{
    Mouse mouse;

    char drop_file[DROP_FILE_PATH_MAX];

    kvec_t(char) text_input;

    bool key_pressed[SDL_SCANCODE_COUNT];
    bool key_just_pressed[SDL_SCANCODE_COUNT];

    Controller controllers[MAX_CONTROLLERS];
} InputManager;

InputManager InputManagerCreate();