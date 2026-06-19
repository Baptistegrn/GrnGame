#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/input/keyboard.h"
#include "wren.h"

static void key_pressed(WrenVM *vm)
{
    int key = (int)wrenGetSlotDouble(vm, 1);
    wrenSetSlotBool(vm, 0, KeyPressed(key));
}

static void key_just_pressed(WrenVM *vm)
{
    int key = (int)wrenGetSlotDouble(vm, 1);
    wrenSetSlotBool(vm, 0, KeyJustPressed(key));
}

static void key_just_released(WrenVM *vm)
{
    int key = (int)wrenGetSlotDouble(vm, 1);
    wrenSetSlotBool(vm, 0, KeyJustReleased(key));
}

void RegisterKeyboardModule()
{
    const char *module = "std/wren/input/keyboard/keyboard";
    const char *cls = "Keyboard";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "pressed(_)", key_pressed);
    RegisterMethod(module, cls, is_static, "just_pressed(_)", key_just_pressed);
    RegisterMethod(module, cls, is_static, "just_released(_)", key_just_released);
}