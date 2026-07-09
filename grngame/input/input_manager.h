#pragma once

#include "controller.h"
#include "grngame/core/param.h"
#include "input_text.h"
#include "mouse.h"
#include <SDL3/SDL_scancode.h>
#include <kvec.h>
#include <stdbool.h>

#include "khash.h"

KHASH_MAP_INIT_INT(joymap, int16)

typedef struct JoystickMap
{
    khash_t(joymap) * hash;
} JoystickMap;

typedef struct
{
    Mouse mouse;

    char drop_file[DROP_FILE_PATH_MAX];

    kvec_t(char) text_input;

    JoystickMap joystick_map;

    bool key_pressed[SDL_SCANCODE_COUNT];
    bool key_just_pressed[SDL_SCANCODE_COUNT];
    bool key_just_released[SDL_SCANCODE_COUNT];

    Controller controllers[MAX_CONTROLLERS];
} InputManager;

InputManager InputManagerCreate();

void JoystickMapAdd(JoystickMap *map, SDL_JoystickID id, int16 index);
int16 JoystickMapGet(const JoystickMap *map, SDL_JoystickID id);
void JoystickMapRemove(JoystickMap *map, SDL_JoystickID id);
void JoystickMapDestroy(JoystickMap *map);