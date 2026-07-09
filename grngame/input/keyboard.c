#include "../core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"

bool KeyPressed(int32 keyboard_index, int32 key)
{
    if (UNLIKELY(keyboard_index < 0 || keyboard_index >= MAX_KEYBOARDS))
    {
        LOG_WARNING("Invalid keyboard index: %d", keyboard_index);
        return false;
    }

    if (UNLIKELY(key < 0 || key >= SDL_SCANCODE_COUNT))
    {
        LOG_WARNING("Invalid scancode: %d", key);
        return false;
    }

    return g_app.input_manager.keyboard[keyboard_index].key_pressed[key];
}

bool KeyJustPressed(int32 keyboard_index, int32 key)
{
    if (UNLIKELY(keyboard_index < 0 || keyboard_index >= MAX_KEYBOARDS))
    {
        LOG_WARNING("Invalid keyboard index: %d", keyboard_index);
        return false;
    }

    if (UNLIKELY(key < 0 || key >= SDL_SCANCODE_COUNT))
    {
        LOG_WARNING("Invalid scancode: %d", key);
        return false;
    }

    return g_app.input_manager.keyboard[keyboard_index].key_just_pressed[key];
}

bool KeyJustReleased(int32 keyboard_index, int32 key)
{
    if (UNLIKELY(keyboard_index < 0 || keyboard_index >= MAX_KEYBOARDS))
    {
        LOG_WARNING("Invalid keyboard index: %d", keyboard_index);
        return false;
    }

    if (UNLIKELY(key < 0 || key >= SDL_SCANCODE_COUNT))
    {
        LOG_WARNING("Invalid scancode: %d", key);
        return false;
    }

    return g_app.input_manager.keyboard[keyboard_index].key_just_released[key];
}