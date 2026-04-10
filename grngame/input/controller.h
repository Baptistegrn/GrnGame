#pragma once

#include "../math/types.h"
#include "SDL3/SDL_gamepad.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

#define MAX_CONTROLLERS 8
#define DEFAULT_DEAD_ZONE 1.0

typedef struct
{
    SDL_Gamepad *gamepad;
    SDL_Joystick *joystick;
    float32 stick_lx, stick_ly;
    float32 stick_rx, stick_ry;
    float32 trigger_l, trigger_r;
    bool pressed[SDL_GAMEPAD_BUTTON_COUNT];
    bool just_pressed[SDL_GAMEPAD_BUTTON_COUNT];
} Controller;

BEGIN_DECLARATIONS;

bool ControllerOpen(int16 index);
void ControllerClose(int16 index);
int ControllerConnectedCountptr(SDL_JoystickID **ptr);
int ControllerConnectedCount(void);

bool PadPressed(int button, int16 index);
bool PadJustPressed(int button, int16 index);

int PadFirstPressedIndexForButton(int button);

SDL_Gamepad *GamepadOpen(SDL_JoystickID id);
SDL_Gamepad *GamepadFromID(SDL_JoystickID id);
SDL_Joystick *GamepadGetJoystick(SDL_Gamepad *gp);
const char *GamepadGetName(SDL_Gamepad *gp);
void GamepadClose(SDL_Gamepad *gp);

void PadSticks(uint16 index, float32 *RESTRICT out_lx, float32 *RESTRICT out_ly, float32 *RESTRICT out_rx,
               float32 *RESTRICT out_ry);
void PadTriggers(uint8 index, float32 *RESTRICT out_left, float32 *RESTRICT out_right);

float32 compute_axis(Sint16 value, float32 dead_zone);

END_DECLARATIONS;