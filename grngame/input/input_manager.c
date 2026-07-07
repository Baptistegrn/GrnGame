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

void ControllerMapAdd(ControllerMap *map, const char *key, int16 index)
{
    int32 ret;
    khint_t k = kh_put(ctrlmap, map->hash, key, &ret);

    if (ret != 0)
    {
        kh_key(map->hash, k) = SDL_strdup(key);
    }
    kh_value(map->hash, k) = index;
}

int16 ControllerMapGet(const ControllerMap *map, const char *key)
{
    khint_t k = kh_get(ctrlmap, map->hash, key);

    if (UNLIKELY(k == kh_end(map->hash)))
        return -1;

    return kh_value(map->hash, k);
}

void ControllerMapDestroy(ControllerMap *map)
{
    if (LIKELY(map->hash))
    {
        for (khint_t k = kh_begin(map->hash); k != kh_end(map->hash); ++k)
        {
            if (kh_exist(map->hash, k))
                SDL_free((void *)kh_key(map->hash, k));
        }
        kh_destroy(ctrlmap, map->hash);
        map->hash = NULL;
    }
}

void ControllerMapRemove(ControllerMap *map, const char *key)
{
    khint_t k = kh_get(ctrlmap, map->hash, key);
    if (LIKELY(k != kh_end(map->hash)))
    {
        SDL_free((void *)kh_key(map->hash, k));
        kh_del(ctrlmap, map->hash, k);
    }
}