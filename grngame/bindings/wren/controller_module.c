
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/input/controller.h"
#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include "wren.h"

static void controller_pad_pressed(WrenVM *vm)
{
    int button = (int)wrenGetSlotDouble(vm, 1);
    int16 index = (int16)wrenGetSlotDouble(vm, 2);
    wrenSetSlotBool(vm, 0, PadPressed(button, index));
}

static void controller_pad_just_pressed(WrenVM *vm)
{
    int button = (int)wrenGetSlotDouble(vm, 1);
    int16 index = (int16)wrenGetSlotDouble(vm, 2);
    wrenSetSlotBool(vm, 0, PadJustPressed(button, index));
}

static void controller_pad_just_released(WrenVM *vm)
{
    int button = (int)wrenGetSlotDouble(vm, 1);
    int16 index = (int16)wrenGetSlotDouble(vm, 2);
    wrenSetSlotBool(vm, 0, PadJustReleased(button, index));
}

static void controller_pad_first_pressed_index_for_button(WrenVM *vm)
{
    int button = (int)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadFirstPressedIndexForButton(button));
}

static void controller_pad_stick_lx(WrenVM *vm)
{
    uint16 index = (uint16)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadStickLX(index));
}

static void controller_pad_stick_ly(WrenVM *vm)
{
    uint16 index = (uint16)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadStickLY(index));
}

static void controller_pad_stick_rx(WrenVM *vm)
{
    uint16 index = (uint16)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadStickRX(index));
}

static void controller_pad_stick_ry(WrenVM *vm)
{
    uint16 index = (uint16)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadStickRY(index));
}

static void controller_pad_trigger_l(WrenVM *vm)
{
    uint8 index = (uint8)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadTriggerL(index));
}

static void controller_pad_trigger_r(WrenVM *vm)
{
    uint8 index = (uint8)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (double)PadTriggerR(index));
}

static void controller_open(WrenVM *vm)
{
    uint8 index = (uint8)wrenGetSlotDouble(vm, 1);
    (void)index;
    ControllerOpen();
}

static void controller_connected_count(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)ControllerConnectedCount());
}

static void pad_rumble(WrenVM *vm)
{
    int16 index = (int16)wrenGetSlotDouble(vm, 1);
    uint16 left_rumble = (uint8)wrenGetSlotDouble(vm, 2);
    uint16 right_rumble = (uint8)wrenGetSlotDouble(vm, 3);
    uint32 time = (uint32)wrenGetSlotDouble(vm, 4);
    PadRumble(index, left_rumble, right_rumble, time);
}

static void controller_pad_sticks_triggers(WrenVM *vm)
{
    uint16 index = (uint16)wrenGetSlotDouble(vm, 1);

    wrenEnsureSlots(vm, 2);
    wrenSetSlotNewList(vm, 0);

    wrenSetSlotDouble(vm, 1, PadStickLX(index));
    wrenInsertInList(vm, 0, -1, 1);

    wrenSetSlotDouble(vm, 1, PadStickLY(index));
    wrenInsertInList(vm, 0, -1, 1);

    wrenSetSlotDouble(vm, 1, PadStickRX(index));
    wrenInsertInList(vm, 0, -1, 1);

    wrenSetSlotDouble(vm, 1, PadStickRY(index));
    wrenInsertInList(vm, 0, -1, 1);

    wrenSetSlotDouble(vm, 1, PadTriggerL(index));
    wrenInsertInList(vm, 0, -1, 1);

    wrenSetSlotDouble(vm, 1, PadTriggerR(index));
    wrenInsertInList(vm, 0, -1, 1);
}

static void  controller_pad_disable_persistence(WrenVM *vm){
    ControllerDisablePersistence((uint16)wrenGetSlotDouble(vm, 1));
}

void RegisterControllerModule()
{
    const char *module = "std/wren/input/pad/pad";
    const char *cls = "Pad";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "pressed(_,_)", controller_pad_pressed);
    RegisterMethod(module, cls, is_static, "just_pressed(_,_)", controller_pad_just_pressed);
    RegisterMethod(module, cls, is_static, "just_released(_,_)", controller_pad_just_released);
    RegisterMethod(module, cls, is_static, "first_pressed_index_for_button(_)",
                   controller_pad_first_pressed_index_for_button);
    RegisterMethod(module, cls, is_static, "pad_rumble(_,_,_,_)", pad_rumble);
    RegisterMethod(module, cls, is_static, "stick_lx(_)", controller_pad_stick_lx);
    RegisterMethod(module, cls, is_static, "stick_ly(_)", controller_pad_stick_ly);
    RegisterMethod(module, cls, is_static, "stick_rx(_)", controller_pad_stick_rx);
    RegisterMethod(module, cls, is_static, "stick_ry(_)", controller_pad_stick_ry);
    RegisterMethod(module, cls, is_static, "trigger_l(_)", controller_pad_trigger_l);
    RegisterMethod(module, cls, is_static, "trigger_r(_)", controller_pad_trigger_r);
    RegisterMethod(module, cls, is_static, "controller_open(_)", controller_open);
    RegisterMethod(module, cls, is_static, "connected_count()", controller_connected_count);
    RegisterMethod(module, cls, is_static, "sticks_triggers(_)", controller_pad_sticks_triggers);
    RegisterMethod(module, cls, is_static, "disable_persistence(_)", controller_pad_disable_persistence);
}