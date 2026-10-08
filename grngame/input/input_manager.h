#pragma once

#include "controller.h"
#include "grngame/core/param.h"
#include "input_text.h"
#include "mouse.h"
#include <SDL3/SDL_scancode.h>
#include <kvec.h>
#include <stdbool.h>

#include "grngame/input/keyboard.h"
#include "khash.h"

typedef struct
{
    Mouse mouse;

    char drop_file[DROP_FILE_PATH_MAX];

    kvec_t(char) text_input;

    Controller controllers[MAX_CONTROLLERS];
    Keyboard keyboard[MAX_KEYBOARDS];
} InputManager;

InputManager InputManagerCreate();
void InputManagerDestroy(InputManager *manager);
