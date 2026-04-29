#include "grngame/bindings/wren/mouse_module.hpp"
#include "cglm/types-struct.h"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/input/mouse.h"
#include <cstring>
#include <wren.h>

// todo create 2 c function for mouse position
static void get_mouse_position_x(WrenVM *vm)
{
    vec2s res = GetMousePosition();
    wren_set<double>(vm, 0, res.x);
}

static void get_mouse_position_y(WrenVM *vm)
{
    vec2s res = GetMousePosition();
    wren_set<double>(vm, 0, res.y);
}

static void mouse_left_pressed(WrenVM *vm)
{
    wren_set<bool>(vm, 0, MouseLeftPressed());
}

static void mouse_left_just_pressed(WrenVM *vm)
{
    wren_set<bool>(vm, 0, MouseLeftJustPressed());
}

static void mouse_right_pressed(WrenVM *vm)
{
    wren_set<bool>(vm, 0, MouseRightPressed());
}

static void mouse_right_just_pressed(WrenVM *vm)
{
    wren_set<bool>(vm, 0, MouseRightJustPressed());
}

static void mouse_x(WrenVM *vm)
{
    wren_set<int>(vm, 0, MouseX());
}

static void mouse_y(WrenVM *vm)
{
    wren_set<int>(vm, 0, MouseY());
}

static void mouse_scroll_x(WrenVM *vm)
{
    wren_set<int>(vm, 0, MouseScrollX());
}

static void mouse_scroll_y(WrenVM *vm)
{
    wren_set<int>(vm, 0, MouseScrollY());
}

static void move_mouse(WrenVM *vm)
{
    int16 x = (int16)wren_get<int>(vm, 1);
    int16 y = (int16)wren_get<int>(vm, 2);
    MoveMouse(x, y);
}

static void show_cursor(WrenVM *vm)
{
    wren_set<bool>(vm, 0, ShowCursor());
}

static void hide_cursor(WrenVM *vm)
{
    wren_set<bool>(vm, 0, HideCursor());
}

WrenForeignClassMethods BindForeignClassCallbackMouse(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    (void)module;
    (void)class_name;
    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackMouse(WrenVM *vm, const char *module, const char *class_name,
                                                   bool is_static, const char *signature)
{
    (void)vm;
    if (!module || !class_name || !signature)
        return nullptr;

    if (std::strcmp(module, "std/wren/input/mouse") == 0)
    {
        if (std::strcmp(class_name, "Mouse") == 0 && is_static)
        {
            if (std::strcmp(signature, "left_pressed()") == 0)
                return mouse_left_pressed;
            if (std::strcmp(signature, "left_just_pressed()") == 0)
                return mouse_left_just_pressed;
            if (std::strcmp(signature, "right_pressed()") == 0)
                return mouse_right_pressed;
            if (std::strcmp(signature, "right_just_pressed()") == 0)
                return mouse_right_just_pressed;
            if (std::strcmp(signature, "x()") == 0)
                return mouse_x;
            if (std::strcmp(signature, "y()") == 0)
                return mouse_y;
            if (std::strcmp(signature, "scroll_x()") == 0)
                return mouse_scroll_x;
            if (std::strcmp(signature, "scroll_y()") == 0)
                return mouse_scroll_y;
            if (std::strcmp(signature, "move_mouse(_,_)") == 0)
                return move_mouse;
            if (std::strcmp(signature, "show_cursor()") == 0)
                return show_cursor;
            if (std::strcmp(signature, "hide_cursor()") == 0)
                return hide_cursor;
            if (std::strcmp(signature, "get_mouse_position_x()") == 0)
                return get_mouse_position_x;
            if (std::strcmp(signature, "get_mouse_position_y()") == 0)
                return get_mouse_position_y;
        }
    }

    return nullptr;
}