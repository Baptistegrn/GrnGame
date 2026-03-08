#pragma once

#include "../math/types.h"
#include "SDL3/SDL_gamepad.h"
#include <stdbool.h>

#define MAX_CONTROLLERS 8

typedef struct
{
    SDL_Gamepad *gamepad;
    SDL_Joystick *joystick;
    int32 stick_lx, stick_ly;
    int32 stick_rx, stick_ry;
    int32 trigger_l, trigger_r;
    bool pressed[SDL_GAMEPAD_BUTTON_COUNT];
    bool just_pressed[SDL_GAMEPAD_BUTTON_COUNT];
} Controller;

bool ControllerOpen(int16 index);
void ControllerClose(int16 index);

bool PadPressed(int button, int16 index);
bool PadJustPressed(int button, int16 index);

SDL_Gamepad *GamepadOpen(SDL_JoystickID id);
SDL_Gamepad *GamepadFromID(SDL_JoystickID id);
SDL_Joystick *GamepadGetJoystick(SDL_Gamepad *gp);
const char *GamepadGetName(SDL_Gamepad *gp);
void GamepadClose(SDL_Gamepad *gp);

void PadSticks(uint16 index, float32 dead_zone, float32 *out_lx, float32 *out_ly, float32 *out_rx, float32 *out_ry);
void PadTriggers(uint8 index, float32 dead_zone, float32 *out_left, float32 *out_right);