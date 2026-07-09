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
        .joystick_map = {.hash = kh_init(joymap)},
    };
    kv_init(m.text_input);
    return m;
}

void JoystickMapAdd(JoystickMap *map, SDL_JoystickID id, int16 index)
{
    int32 ret;
    khint_t k = kh_put(joymap, map->hash, (khint32_t)id, &ret);

    if (ret != -1)
    {
        kh_value(map->hash, k) = index;
    }
}

int16 JoystickMapGet(const JoystickMap *map, SDL_JoystickID id)
{
    khint_t k = kh_get(joymap, map->hash, (khint32_t)id);

    if (UNLIKELY(k == kh_end(map->hash)))
        return -1;

    return kh_value(map->hash, k);
}

void JoystickMapDestroy(JoystickMap *map)
{
    if (LIKELY(map->hash))
    {
        kh_destroy(joymap, map->hash);
        map->hash = NULL;
    }
}

void JoystickMapRemove(JoystickMap *map, SDL_JoystickID id)
{
    khint_t k = kh_get(joymap, map->hash, (khint32_t)id);
    if (LIKELY(k != kh_end(map->hash)))
    {
        kh_del(joymap, map->hash, k);
    }
}