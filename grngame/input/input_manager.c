#include "input_manager.h"
#include "grngame/utils/attributes.h"

InputManager InputManagerCreate()
{
    InputManager m = {
        .mouse = {.x = 0,
                  .y = 0,
                  .left_pressed = false,
                  .left_just_pressed = false,
                  .left_just_released = false,
                  .right_pressed = false,
                  .right_just_pressed = false,
                  .right_just_released = false,
                  .scroll_x = 0,
                  .scroll_y = 0},
        .drop_file = {0},
        .key_pressed = {false},
        .key_just_pressed = {false},
        .controllers = {0},
        .controller_map = {.hash = kh_init(ctrlmap)},
    };
    kv_init(m.text_input);
    return m;
}

void ControllerMapDestroy(ControllerMap *map)
{
    if (LIKELY(map->hash))
    {
        kh_destroy(ctrlmap, map->hash);
        map->hash = NULL;
    }
}

void ControllerMapAdd(ControllerMap *map, SDL_JoystickID id, int16 index)
{
    int ret;
    khint_t k = kh_put(ctrlmap, map->hash, (khint32_t)id, &ret);
    kh_value(map->hash, k) = index;
}

int16 ControllerMapGet(const ControllerMap *map, SDL_JoystickID id)
{
    khint_t k = kh_get(ctrlmap, map->hash, (khint32_t)id);
    if (UNLIKELY(k == kh_end(map->hash)))
        return -1;
    return kh_value(map->hash, k);
}

void ControllerMapRemove(ControllerMap *map, SDL_JoystickID id)
{
    khint_t k = kh_get(ctrlmap, map->hash, (khint32_t)id);
    if (LIKELY(k != kh_end(map->hash)))
        kh_del(ctrlmap, map->hash, k);
}