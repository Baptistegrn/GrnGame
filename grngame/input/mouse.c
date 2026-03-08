#include "../core/app.h"
#include "cglm/types-struct.h"
#include <SDL3/SDL_mouse.h>

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
