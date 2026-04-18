#include "grngame/bindings/wren/controller_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/input/controller.h"
#include "grngame/utils/attributes.h"
#include <cstring>

static void controller_pad_pressed(WrenVM *vm)
{
    int button = wren_get<int>(vm, 1);
    int16 index = (int16)wren_get<int>(vm, 2);
    wren_set(vm, 0, PadPressed(button, index));
}

static void controller_pad_just_pressed(WrenVM *vm)
{
    int button = wren_get<int>(vm, 1);
    int16 index = (int16)wren_get<int>(vm, 2);
    wren_set(vm, 0, PadJustPressed(button, index));
}

static void controller_pad_first_pressed_index_for_button(WrenVM *vm)
{
    int button = wren_get<int>(vm, 1);
    wren_set<int>(vm, 0, PadFirstPressedIndexForButton(button));
}

static void controller_pad_sticks(WrenVM *vm)
{
    uint16 index = (uint16)wren_get<int>(vm, 1);
    float32 lx = 0.0f, ly = 0.0f, rx = 0.0f, ry = 0.0f;
    PadSticks(index, &lx, &ly, &rx, &ry);

    wrenEnsureSlots(vm, 5);
    wrenSetSlotNewList(vm, 0);

    wrenSetSlotDouble(vm, 1, lx);
    wrenInsertInList(vm, 0, -1, 1);
    wrenSetSlotDouble(vm, 2, ly);
    wrenInsertInList(vm, 0, -1, 2);
    wrenSetSlotDouble(vm, 3, rx);
    wrenInsertInList(vm, 0, -1, 3);
    wrenSetSlotDouble(vm, 4, ry);
    wrenInsertInList(vm, 0, -1, 4);
}

static void controller_pad_triggers(WrenVM *vm)
{
    uint8 index = (uint8)wren_get<int>(vm, 1);
    float32 l = 0.0f, r = 0.0f;
    PadTriggers(index, &l, &r);

    wrenEnsureSlots(vm, 3);
    wrenSetSlotNewList(vm, 0);

    wrenSetSlotDouble(vm, 1, l);
    wrenInsertInList(vm, 0, -1, 1);
    wrenSetSlotDouble(vm, 2, r);
    wrenInsertInList(vm, 0, -1, 2);
}

WrenForeignClassMethods BindForeignClassCallbackController(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    return {nullptr, nullptr};
}

static void controller_open(WrenVM *vm)
{
    uint8 index = (uint8)wren_get<int>(vm, 1);
    ControllerOpen();
}

static void controller_connected_count(WrenVM *vm)
{
    wren_set<int>(vm, 0, ControllerConnectedCount());
}

WrenForeignMethodFn BindForeignMethodCallbackController(WrenVM *vm, const char *module, const char *class_name,
                                                        bool is_static, const char *signature)
{
    (void)vm;
    if (UNLIKELY(!module || !class_name || !signature))
        return nullptr;

    if (strcmp(module, "std/wren/input/pad_node") == 0)
    {
        if (strcmp(class_name, "PadNode") == 0 && is_static)
        {
            if (strcmp(signature, "pressed(_,_)") == 0)
                return controller_pad_pressed;
            if (strcmp(signature, "just_pressed(_,_)") == 0)
                return controller_pad_just_pressed;
            if (strcmp(signature, "first_pressed_index_for_button(_)") == 0)
                return controller_pad_first_pressed_index_for_button;
            if (strcmp(signature, "sticks(_)") == 0)
                return controller_pad_sticks;
            if (strcmp(signature, "triggers(_)") == 0)
                return controller_pad_triggers;
            if (strcmp(signature, "controller_open(_)") == 0)
                return controller_open;
            if (strcmp(signature, "connected_count()") == 0)
                return controller_connected_count;
        }
    }

    return nullptr;
}