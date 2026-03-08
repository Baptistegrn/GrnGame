#include "../core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL_events.h>

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

    SDL_Gamepad *gp = GamepadOpen(index);
    if (UNLIKELY(!gp))
        return false;

    SDL_Joystick *joy = GamepadGetJoystick(gp);

    g_app.input_manager.controllers[index].gamepad = gp;
    g_app.input_manager.controllers[index].joystick = joy;
    LOG_INFO("Gamepad %d opened: %s", index, GamepadGetName(gp));
    return true;
}

void ControllerClose(int16 index)
{
    if (index >= MAX_CONTROLLERS)
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
    if (index >= MAX_CONTROLLERS || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT)
    {
        LOG_WARNING("Invalid pad button %d or index %d", button, index);
        return false;
    }

    return g_app.input_manager.controllers[index].pressed[button];
}

bool PadJustPressed(int button, int16 index)
{
    if (index >= MAX_CONTROLLERS || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT)
    {
        LOG_WARNING("Invalid pad button %d or index %d", button, index);
        return false;
    }
    return g_app.input_manager.controllers[index].just_pressed[button];
}

void PadSticks(uint16 index, float32 dead_zone, float32 *out_lx, float32 *out_ly, float32 *out_rx, float32 *out_ry)
{
    if (index >= MAX_CONTROLLERS)
    {
        *out_lx = *out_ly = *out_rx = *out_ry = 0.0f;
        LOG_WARNING("Invalid controller index for sticks: %d", index);
        return;
    }

    const float norm = 32767.0f;
    Controller *c = &g_app.input_manager.controllers[index];

    float32 lx = (float32)c->stick_lx / norm;
    float32 ly = (float32)c->stick_ly / norm;
    float32 rx = (float32)c->stick_rx / norm;
    float32 ry = (float32)c->stick_ry / norm;

    *out_lx = fabsf(lx) < dead_zone ? 0.0f : lx;
    *out_ly = fabsf(ly) < dead_zone ? 0.0f : ly;
    *out_rx = fabsf(rx) < dead_zone ? 0.0f : rx;
    *out_ry = fabsf(ry) < dead_zone ? 0.0f : ry;
}

void PadTriggers(uint8 index, float32 dead_zone, float32 *out_left, float32 *out_right)
{
    if (index >= MAX_CONTROLLERS)
    {
        *out_left = *out_right = 0.0f;
        LOG_WARNING("Invalid controller index for triggers: %d", index);
        return;
    }

    const float32 norm = 32767.0f;
    Controller *c = &g_app.input_manager.controllers[index];

    float32 l = (float32)c->trigger_l / norm;
    float32 r = (float)c->trigger_r / norm;

    *out_left = fabsf(l) < dead_zone ? 0.0f : l;
    *out_right = fabsf(r) < dead_zone ? 0.0f : r;
}
