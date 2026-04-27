#pragma once

#include "controller.h"
#include "input_text.h"
#include "mouse.h"
#include <SDL3/SDL_scancode.h>
#include <kvec.h>
#include <stdbool.h>

typedef struct
{
    const char *name;
    int16 index;
} ControllerMapEntry;

typedef struct
{
    ControllerMapEntry entries[MAX_CONTROLLERS];
    int16 count;
} ControllerMap;

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

void ControllerMapAdd(ControllerMap *map, const char *name, int16 index);
int16 ControllerMapGet(const ControllerMap *map, const char *name);
void ControllerMapRemove(ControllerMap *map, const char *name);