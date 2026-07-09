#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/input/keyboard.h"
#include "wren.h"

static void key_pressed(WrenVM *vm)
{
    int32 keyboard_index = (int32)wrenGetSlotDouble(vm, 1);
    int32 key = (int32)wrenGetSlotDouble(vm, 2);
    wrenSetSlotBool(vm, 0, KeyPressed(keyboard_index, key));
}

static void key_just_pressed(WrenVM *vm)
{
    int32 keyboard_index = (int32)wrenGetSlotDouble(vm, 1);
    int32 key = (int32)wrenGetSlotDouble(vm, 2);
    wrenSetSlotBool(vm, 0, KeyJustPressed(keyboard_index, key));
}

static void key_just_released(WrenVM *vm)
{
    int32 keyboard_index = (int32)wrenGetSlotDouble(vm, 1);
    int32 key = (int32)wrenGetSlotDouble(vm, 2);
    wrenSetSlotBool(vm, 0, KeyJustReleased(keyboard_index, key));
}

void RegisterKeyboardModule()
{
    const char *module = "std/wren/input/keyboard/keyboard";
    const char *cls = "Keyboard";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "pressed(_,_)", key_pressed);
    RegisterMethod(module, cls, is_static, "just_pressed(_,_)", key_just_pressed);
    RegisterMethod(module, cls, is_static, "just_released(_,_)", key_just_released);
}