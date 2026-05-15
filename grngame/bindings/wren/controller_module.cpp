#include "grngame/bindings/wren/controller_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/input/controller.h"
#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
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

static void controller_pad_just_released(WrenVM *vm)
{
    int button = wren_get<int>(vm, 1);
    int16 index = (int16)wren_get<int>(vm, 2);
    wren_set(vm, 0, PadJustReleased(button, index));
}

static void controller_pad_first_pressed_index_for_button(WrenVM *vm)
{
    int button = wren_get<int>(vm, 1);
    wren_set<int>(vm, 0, PadFirstPressedIndexForButton(button));
}

static void controller_pad_stick_lx(WrenVM *vm)
{
    uint16 index = (uint16)wren_get<int>(vm, 1);
    wren_set<double>(vm, 0, PadStickLX(index));
}

static void controller_pad_stick_ly(WrenVM *vm)
{
    uint16 index = (uint16)wren_get<int>(vm, 1);
    wren_set<double>(vm, 0, PadStickLY(index));
}

static void controller_pad_stick_rx(WrenVM *vm)
{
    uint16 index = (uint16)wren_get<int>(vm, 1);
    wren_set<double>(vm, 0, PadStickRX(index));
}

static void controller_pad_stick_ry(WrenVM *vm)
{
    uint16 index = (uint16)wren_get<int>(vm, 1);
    wren_set<double>(vm, 0, PadStickRY(index));
}

static void controller_pad_trigger_l(WrenVM *vm)
{
    uint8 index = (uint8)wren_get<int>(vm, 1);
    wren_set<double>(vm, 0, PadTriggerL(index));
}

static void controller_pad_trigger_r(WrenVM *vm)
{
    uint8 index = (uint8)wren_get<int>(vm, 1);
    wren_set<double>(vm, 0, PadTriggerR(index));
}

WrenForeignClassMethods BindForeignClassCallbackController(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    (void)module;
    (void)class_name;
    return {nullptr, nullptr};
}

static void controller_open(WrenVM *vm)
{
    uint8 index = (uint8)wren_get<int>(vm, 1);
    (void)index;
    ControllerOpen();
}

static void controller_connected_count(WrenVM *vm)
{
    wren_set<int>(vm, 0, ControllerConnectedCount());
}

static void pad_rumble(WrenVM *vm)
{
    int16 index = (int16)wren_get<int>(vm, 1);
    uint16 left_rumble = (uint16)wren_get<int>(vm, 2);
    uint16 right_rumble = (uint16)wren_get<int>(vm, 3);
    uint32 time = (uint32)wren_get<int>(vm, 4);
    PadRumble(index, left_rumble, right_rumble, time);
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
            if (strcmp(signature, "just_released(_,_)") == 0)
                return controller_pad_just_released;
            if (strcmp(signature, "first_pressed_index_for_button(_)") == 0)
                return controller_pad_first_pressed_index_for_button;
            if (strcmp(signature, "pad_rumble(_,_,_,_)") == 0)
                return pad_rumble;
            if (strcmp(signature, "stick_lx(_)") == 0)
                return controller_pad_stick_lx;
            if (strcmp(signature, "stick_ly(_)") == 0)
                return controller_pad_stick_ly;
            if (strcmp(signature, "stick_rx(_)") == 0)
                return controller_pad_stick_rx;
            if (strcmp(signature, "stick_ry(_)") == 0)
                return controller_pad_stick_ry;
            if (strcmp(signature, "trigger_l(_)") == 0)
                return controller_pad_trigger_l;
            if (strcmp(signature, "trigger_r(_)") == 0)
                return controller_pad_trigger_r;
            if (strcmp(signature, "controller_open(_)") == 0)
                return controller_open;
            if (strcmp(signature, "connected_count()") == 0)
                return controller_connected_count;
        }
    }

    return nullptr;
}