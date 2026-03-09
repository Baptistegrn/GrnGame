#include "../core/app.h"
#include "SDL3/SDL_error.h"
#include "cglm/types-struct.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include <SDL3/SDL_mouse.h>

#include <math.h>

vec2s GetMousePosition()
{
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    return (vec2s){{mx, my}};
}

bool MouseLeftPressed()
{
    return g_app.input_manager.mouse.left_pressed;
}
bool MouseLeftJustPressed()
{
    return g_app.input_manager.mouse.left_just_pressed;
}
bool MouseRightPressed()
{
    return g_app.input_manager.mouse.right_pressed;
}
bool MouseRightJustPressed()
{
    return g_app.input_manager.mouse.right_just_pressed;
}
int32 MouseX()
{
    return g_app.input_manager.mouse.x;
}
int32 MouseY()
{
    return g_app.input_manager.mouse.y;
}
int8 MouseScrollX()
{
    return g_app.input_manager.mouse.scroll_x;
}
int8 MouseScrollY()
{
    return g_app.input_manager.mouse.scroll_y;
}

void MoveMouse(int16 x, int16 y)
{
    uint8 coeff = WindowGetScale();
    int mx = (int)((x + g_app.info.offset_x) * coeff);
    int my = (int)((y + g_app.info.offset_y) * coeff);
    SDL_WarpMouseInWindow(g_app.window, mx, my);
}

bool HideCursor()
{
    if (!SDL_HideCursor())
    {
        LOG_WARNING("Impossible to hide cursor :%s", SDL_GetError());
        return false;
    }
    return true;
}

bool ShowCursor()
{
    if (!SDL_ShowCursor())
    {
        LOG_WARNING("Impossible to show cursor :%s", SDL_GetError());
        return false;
    }
    return true;
}
