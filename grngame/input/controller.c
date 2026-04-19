#include "controller.h"
#include "../core/app.h"
#include "SDL3/SDL_gamepad.h"
#include "SDL3/SDL_joystick.h"
#include "SDL3/SDL_stdinc.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL_events.h>
#include <math.h>

// sdl wrappent
SDL_Gamepad *GamepadOpen(SDL_JoystickID id)
{
    SDL_Gamepad *gp = SDL_OpenGamepad(id);
    if (UNLIKELY(!gp))
        LOG_WARNING("Failed to open gamepad %d: %s", id, SDL_GetError());
    return gp;
}

SDL_Gamepad *GamepadFromID(SDL_JoystickID id)
{
    return SDL_GetGamepadFromID(id);
}

SDL_Joystick *GamepadGetJoystick(SDL_Gamepad *gp)
{
    SDL_Joystick *joy = SDL_GetGamepadJoystick(gp);
    if (UNLIKELY(!joy))
        LOG_WARNING("Failed to get joystick from gamepad: %s", SDL_GetError());
    return joy;
}

const char *GamepadGetName(SDL_Gamepad *gp)
{
    const char *name = SDL_GetGamepadName(gp);
    if (UNLIKELY(!name))
        LOG_WARNING("Failed to get gamepad name: %s", SDL_GetError());
    return name;
}

void GamepadClose(SDL_Gamepad *gp)
{
    SDL_CloseGamepad(gp);
}

// todo : explain and link index with uniq id of controller not name
bool ControllerOpen()
{
    SDL_JoystickID *pads = NULL;
    int count = ControllerConnectedCountptr(&pads);

    if (UNLIKELY(count == 0))
    {
        return false;
    }

    SDL_JoystickID new_pad_id = 0;
    bool found = false;

    for (int i = 0; i < count; i++)
    {
        SDL_Gamepad *temp_gp = GamepadFromID(pads[i]);
        if (!temp_gp) // Not opened yet
        {
            new_pad_id = pads[i];
            found = true;
            break;
        }
    }

    if (UNLIKELY(!found))
    {
        if (LIKELY(pads))
            SDL_free(pads);
        return false;
    }

    SDL_Gamepad *gp = GamepadOpen(new_pad_id);
    if (LIKELY(pads))
        SDL_free(pads);

    if (UNLIKELY(!gp))
        return false;

    const char *name = GamepadGetName(gp);
    if (UNLIKELY(!name))
        name = "Unknown";

    int16 index = ControllerMapGet(&g_app.input_manager.controller_map, name);
    if (index == -1)
    {
        index = g_app.input_manager.controller_map.count;
        if (UNLIKELY(index >= MAX_CONTROLLERS))
        {
            LOG_WARNING("Max controllers reached");
            GamepadClose(gp);
            return false;
        }
        ControllerMapAdd(&g_app.input_manager.controller_map, SDL_strdup(name), index);
    }

    SDL_Joystick *joy = GamepadGetJoystick(gp);

    g_app.input_manager.controllers[index].gamepad = gp;
    g_app.input_manager.controllers[index].joystick = joy;

    if (UNLIKELY(!SDL_GamepadHasAxis(gp, SDL_GAMEPAD_AXIS_LEFTX) || !SDL_GamepadHasAxis(gp, SDL_GAMEPAD_AXIS_LEFTY)))
        LOG_WARNING("Gamepad %d (%s) is missing a left stick", index, name);
    if (UNLIKELY(!SDL_GamepadHasAxis(gp, SDL_GAMEPAD_AXIS_RIGHTX) || !SDL_GamepadHasAxis(gp, SDL_GAMEPAD_AXIS_RIGHTY)))
        LOG_WARNING("Gamepad %d (%s) is missing a right stick", index, name);
    if (UNLIKELY(!SDL_GamepadHasAxis(gp, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) ||
                 !SDL_GamepadHasAxis(gp, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)))
        LOG_WARNING("Gamepad %d (%s) might be missing triggers", index, name);

    LOG_INFO("Gamepad %d opened: %s", index, name);
    return true;
}

int ControllerConnectedCountptr(SDL_JoystickID **ptr)
{
    int count = 0;
    *ptr = SDL_GetGamepads(&count);
    return count;
}

int ControllerConnectedCount(void)
{
    int count = 0;
    SDL_JoystickID *pads = SDL_GetGamepads(&count);
    if (LIKELY(pads))
        SDL_free(pads);
    return count;
}

void ControllerClose(int16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for close: %d", index);
        return;
    }

    Controller *c = &g_app.input_manager.controllers[index];
    if (c->gamepad)
    {
        GamepadClose(c->gamepad);
        c->gamepad = NULL;
        c->joystick = NULL;
    }
}

bool PadPressed(int button, int16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT))
    {
        LOG_WARNING("Invalid pad button %d or index %d", button, index);
        return false;
    }

    return g_app.input_manager.controllers[index].pressed[button];
}

bool PadJustPressed(int button, int16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT))
    {
        LOG_WARNING("Invalid pad button %d or index %d", button, index);
        return false;
    }
    return g_app.input_manager.controllers[index].just_pressed[button];
}

bool PadJustReleased(int button, int16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT))
    {
        LOG_WARNING("Invalid pad button %d or index %d", button, index);
        return false;
    }
    return g_app.input_manager.controllers[index].just_released[button];
}

bool PadRumble(int16 index, uint16 left_rumble, uint16 right_rumble, uint32 time)
{
    if (UNLIKELY(index < 0 || index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for rumble: %d", index);
        return false;
    }

    SDL_Gamepad *gp = g_app.input_manager.controllers[index].gamepad;
    if (UNLIKELY(!gp))
    {
        LOG_WARNING("Controller %d is not connected or gamepad is null", index);
        return false;
    }

    if (!SDL_RumbleGamepad(gp, left_rumble, right_rumble, time))
    {
        LOG_WARNING("Failed to rumble controller %d: %s", index, SDL_GetError());
        return false;
    }

    return true;
}

bool PadHasButton(int16 index, int button)
{
    if (UNLIKELY(index < 0 || index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for has_button: %d", index);
        return false;
    }

    SDL_Gamepad *gp = g_app.input_manager.controllers[index].gamepad;
    if (UNLIKELY(!gp))
    {
        LOG_WARNING("Controller %d is not connected or gamepad is null", index);
        return false;
    }

    return SDL_GamepadHasButton(gp, (SDL_GamepadButton)button);
}

bool PadHasAxis(int16 index, int axis)
{
    if (UNLIKELY(index < 0 || index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for has_axis: %d", index);
        return false;
    }

    SDL_Gamepad *gp = g_app.input_manager.controllers[index].gamepad;
    if (UNLIKELY(!gp))
    {
        LOG_WARNING("Controller %d is not connected or gamepad is null", index);
        return false;
    }

    return SDL_GamepadHasAxis(gp, (SDL_GamepadAxis)axis);
}

bool PadHasSensor(int16 index, int sensor_type)
{
    if (UNLIKELY(index < 0 || index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for has_sensor: %d", index);
        return false;
    }

    SDL_Gamepad *gp = g_app.input_manager.controllers[index].gamepad;
    if (UNLIKELY(!gp))
    {
        LOG_WARNING("Controller %d is not connected or gamepad is null", index);
        return false;
    }

    return SDL_GamepadHasSensor(gp, (SDL_SensorType)sensor_type);
}

int PadFirstPressedIndexForButton(int button)
{
    if (UNLIKELY(button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT))
    {
        LOG_WARNING("Invalid pad button %d", button);
        return -1;
    }

    for (int i = 0; i < MAX_CONTROLLERS; ++i)
    {
        Controller *c = &g_app.input_manager.controllers[i];
        if (!c->gamepad)
            continue;

        if (c->pressed[button])
            return i;
    }

    return -1;
}

float32 compute_axis(Sint16 value, float32 dead_zone)
{
    const float32 norm = 32767.0f;
    float32 res = (float32)value / norm;
    if (UNLIKELY(fabsf(res) < dead_zone))
    {
        return 0.0f;
    }
    return res;
}

float32 PadStickLX(uint16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for stick lx: %d", index);
        return 0.0f;
    }
    return g_app.input_manager.controllers[index].stick_lx;
}

float32 PadStickLY(uint16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for stick ly: %d", index);
        return 0.0f;
    }
    return g_app.input_manager.controllers[index].stick_ly;
}

float32 PadStickRX(uint16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for stick rx: %d", index);
        return 0.0f;
    }
    return g_app.input_manager.controllers[index].stick_rx;
}

float32 PadStickRY(uint16 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for stick ry: %d", index);
        return 0.0f;
    }
    return g_app.input_manager.controllers[index].stick_ry;
}

float32 PadTriggerL(uint8 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for trigger l: %d", index);
        return 0.0f;
    }
    return g_app.input_manager.controllers[index].trigger_l;
}

float32 PadTriggerR(uint8 index)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        LOG_WARNING("Invalid controller index for trigger r: %d", index);
        return 0.0f;
    }
    return g_app.input_manager.controllers[index].trigger_r;
}
