#include "../core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"

bool KeyPressed(int32 key)
{
    if (UNLIKELY(key < 0 || key >= SDL_SCANCODE_COUNT))
    {
        LOG_WARNING("Invalid scancode: %d", key);
        return false;
    }
    return g_app.input_manager.key_pressed[key];
}

bool KeyJustPressed(int32 key)
{
    if (UNLIKELY(key < 0 || key >= SDL_SCANCODE_COUNT))
    {
        LOG_WARNING("Invalid scancode: %d", key);
        return false;
    }
    return g_app.input_manager.key_just_pressed[key];
}

bool KeyJustReleased(int32 key)
{
    if (UNLIKELY(key < 0 || key >= SDL_SCANCODE_COUNT))
    {
        LOG_WARNING("Invalid scancode: %d", key);
        return false;
    }
    return g_app.input_manager.key_just_released[key];
}
