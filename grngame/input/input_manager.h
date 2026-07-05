#pragma once

#include "controller.h"
#include "grngame/core/param.h"
#include "input_text.h"
#include "mouse.h"
#include <SDL3/SDL_scancode.h>
#include <kvec.h>
#include <stdbool.h>

#include "khash.h"

KHASH_MAP_INIT_STR(ctrlmap, int16)

typedef struct ControllerMap
{
    khash_t(ctrlmap) * hash;
} ControllerMap;

typedef struct
{
    Mouse mouse;

    char drop_file[DROP_FILE_PATH_MAX];

    kvec_t(char) text_input;

    // index is save here for pad with serial number 
    ControllerMap controller_map;

    bool key_pressed[SDL_SCANCODE_COUNT];
    bool key_just_pressed[SDL_SCANCODE_COUNT];
    bool key_just_released[SDL_SCANCODE_COUNT];

    Controller controllers[MAX_CONTROLLERS];
} InputManager;

InputManager InputManagerCreate();

void ControllerMapAdd(ControllerMap *map, const char *key, int16 index);
int16 ControllerMapGet(const ControllerMap *map, const char *key);
void ControllerMapRemove(ControllerMap *map, const char *key);
void ControllerMapDestroy(ControllerMap *map);