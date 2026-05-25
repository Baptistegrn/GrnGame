#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/input/mouse.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <stdbool.h>

static void mouse_left_pressed(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, MouseLeftPressed());
}

static void mouse_left_just_pressed(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, MouseLeftJustPressed());
}

static void mouse_left_just_released(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, MouseLeftJustReleased());
}

static void mouse_right_pressed(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, MouseRightPressed());
}

static void mouse_right_just_pressed(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, MouseRightJustPressed());
}

static void mouse_right_just_released(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, MouseRightJustReleased());
}

static void mouse_x(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)MouseX());
}

static void mouse_y(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)MouseY());
}

static void mouse_scroll_x(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)MouseScrollX());
}

static void mouse_scroll_y(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)MouseScrollY());
}

static void move_mouse(WrenVM *vm)
{
    int16 x = (int16)wrenGetSlotDouble(vm, 1);
    int16 y = (int16)wrenGetSlotDouble(vm, 2);
    MoveMouse(x, y);
}

static void show_cursor(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, ShowCursor());
}

static void hide_cursor(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, HideCursor());
}

void RegisterMouseModule()
{
    const char *module = "std/wren/input/mouse";
    const char *cls = "Mouse";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "left_pressed()", mouse_left_pressed);
    RegisterMethod(module, cls, is_static, "left_just_pressed()", mouse_left_just_pressed);
    RegisterMethod(module, cls, is_static, "left_just_released()", mouse_left_just_released);
    RegisterMethod(module, cls, is_static, "right_pressed()", mouse_right_pressed);
    RegisterMethod(module, cls, is_static, "right_just_pressed()", mouse_right_just_pressed);
    RegisterMethod(module, cls, is_static, "right_just_released()", mouse_right_just_released);
    RegisterMethod(module, cls, is_static, "x()", mouse_x);
    RegisterMethod(module, cls, is_static, "y()", mouse_y);
    RegisterMethod(module, cls, is_static, "scroll_x()", mouse_scroll_x);
    RegisterMethod(module, cls, is_static, "scroll_y()", mouse_scroll_y);
    RegisterMethod(module, cls, is_static, "move_mouse(_,_)", move_mouse);
    RegisterMethod(module, cls, is_static, "show_cursor()", show_cursor);
    RegisterMethod(module, cls, is_static, "hide_cursor()", hide_cursor);
}