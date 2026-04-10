#include "controller.h"
#include "../core/app.h"
#include "SDL3/SDL_joystick.h"
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
    SDL_Gamepad *gp = SDL_GetGamepadFromID(id);
    if (UNLIKELY(!gp))
        LOG_WARNING("Failed to get gamepad from ID %d: %s", id, SDL_GetError());
    return gp;
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

bool ControllerOpen(int16 index)
{
    if (index >= MAX_CONTROLLERS)
        return false;
    SDL_JoystickID *pads = NULL;
    int count = ControllerConnectedCountptr(&pads);

    if (UNLIKELY(count == 0))
    {
        return false;
    }
    index = (int16)count - 1;

    SDL_Gamepad *gp = GamepadOpen(pads[index]);
    if (LIKELY(pads))
        SDL_free(pads);

    if (UNLIKELY(!gp))
        return false;

    SDL_Joystick *joy = GamepadGetJoystick(gp);

    g_app.input_manager.controllers[index].gamepad = gp;
    g_app.input_manager.controllers[index].joystick = joy;

    LOG_INFO("Gamepad %d opened: %s", index, GamepadGetName(gp));
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

void PadSticks(uint16 index, float32 *RESTRICT out_lx, float32 *RESTRICT out_ly, float32 *RESTRICT out_rx,
               float32 *RESTRICT out_ry)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        *out_lx = *out_ly = *out_rx = *out_ry = 0.0f;
        LOG_WARNING("Invalid controller index for sticks: %d", index);
        return;
    }

    Controller *c = &g_app.input_manager.controllers[index];

    *out_lx = c->stick_lx;
    *out_ly = c->stick_ly;
    *out_rx = c->stick_rx;
    *out_ry = c->stick_ry;
}

void PadTriggers(uint8 index, float32 *RESTRICT out_left, float32 *RESTRICT out_right)
{
    if (UNLIKELY(index >= MAX_CONTROLLERS))
    {
        *out_left = *out_right = 0.0f;
        LOG_WARNING("Invalid controller index for triggers: %d", index);
        return;
    }

    Controller *c = &g_app.input_manager.controllers[index];

    *out_left = c->trigger_l;
    *out_right = c->trigger_r;
}
