#include "input_manager.h"
#include "controller.h"
#include "grngame/dev/logging.h"
#include <string.h>

InputManager InputManagerCreate()
{
    InputManager m = {
        .mouse = {.x = 0,
                  .y = 0,
                  .left_pressed = false,
                  .left_just_pressed = false,
                  .right_pressed = false,
                  .right_just_pressed = false,
                  .scroll_x = 0,
                  .scroll_y = 0},
        .drop_file = {0},
        .key_pressed = {false},
        .key_just_pressed = {false},
        .controllers = {0},
    };
    kv_init(m.text_input);
    return m;
}

void ControllerMapAdd(ControllerMap *map, const char *name, int16 index)
{
    int16 count = map->count;
    if (LIKELY(count < MAX_CONTROLLERS))
    {
        map->entries[count].name = name;
        map->entries[count].index = index;
        map->count++;
    }
    else
    {
        LOG_WARNING("trying to add another controller to controller map but its already full");
    }
}

int16 ControllerMapGet(const ControllerMap *map, const char *name)
{
    for (int16 i = 0; i < map->count; ++i)
    {
        if (strcmp(map->entries[i].name, name) == 0)
        {
            return map->entries[i].index;
        }
    }
    return -1;
}

void ControllerMapRemove(ControllerMap *map, const char *name)
{
    for (int16 i = 0; i < map->count; ++i)
    {
        if (strcmp(map->entries[i].name, name) == 0)
        {
            map->count--;
            map->entries[i] = map->entries[map->count];
            return;
        }
    }
}