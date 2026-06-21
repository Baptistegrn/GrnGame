#pragma once

#include "controller.h"
#include "grngame/core/param.h"
#include "input_text.h"
#include "mouse.h"
#include <SDL3/SDL_scancode.h>
#include <kvec.h>
#include <stdbool.h>

#include "khash.h"

KHASH_MAP_INIT_INT(ctrlmap, int16)

typedef struct ControllerMap
{
    khash_t(ctrlmap) * hash;
} ControllerMap;

typedef struct
{
    const char *name;
    int16 index;
} ControllerMapEntry;

typedef struct
{
    Mouse mouse;

    char drop_file[DROP_FILE_PATH_MAX];

    kvec_t(char) text_input;

    ControllerMap controller_map;

    bool key_pressed[SDL_SCANCODE_COUNT];
    bool key_just_pressed[SDL_SCANCODE_COUNT];
    bool key_just_released[SDL_SCANCODE_COUNT];

    Controller controllers[MAX_CONTROLLERS];
} InputManager;

InputManager InputManagerCreate();

void ControllerMapAdd(ControllerMap *map, SDL_JoystickID id, int16 index);
int16 ControllerMapGet(const ControllerMap *map, SDL_JoystickID id);
void ControllerMapRemove(ControllerMap *map, SDL_JoystickID id);