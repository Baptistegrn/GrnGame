#pragma once

#include "../math/types.h"
#include "SDL3/SDL_gamepad.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

typedef struct
{
    SDL_Gamepad *gamepad;
    SDL_Joystick *joystick;
    SDL_JoystickID id;
    float32 stick_lx, stick_ly;
    float32 stick_rx, stick_ry;
    float32 trigger_l, trigger_r;
    bool pressed[SDL_GAMEPAD_BUTTON_COUNT];
    bool just_pressed[SDL_GAMEPAD_BUTTON_COUNT];
    bool just_released[SDL_GAMEPAD_BUTTON_COUNT];
    const char *name;

} Controller;

BEGIN_DECLARATIONS;

bool ControllerOpen();
void ControllerClose(int16 index);
int ControllerConnectedCountptr(SDL_JoystickID **ptr);
int ControllerConnectedCount(void);

bool PadPressed(int button, int16 index);
bool PadJustPressed(int button, int16 index);
bool PadJustReleased(int button, int16 index);

int PadFirstPressedIndexForButton(int button);

SDL_Gamepad *GamepadOpen(SDL_JoystickID id);
SDL_Gamepad *GamepadFromID(SDL_JoystickID id);
SDL_Joystick *GamepadGetJoystick(SDL_Gamepad *gp);
const char *GamepadGetName(SDL_Gamepad *gp);
void GamepadClose(SDL_Gamepad *gp);

float32 PadStickLX(uint16 index);
float32 PadStickLY(uint16 index);
float32 PadStickRX(uint16 index);
float32 PadStickRY(uint16 index);

float32 PadTriggerL(uint8 index);
float32 PadTriggerR(uint8 index);

bool PadRumble(int16 index, uint8 left_rumble, uint8 right_rumble, uint32 duration_ms);
bool PadHasButton(int16 index, int button);
bool PadHasAxis(int16 index, int axis);
bool PadHasSensor(int16 index, int sensor_type);

float32 compute_axis(Sint16 value, float32 dead_zone);

END_DECLARATIONS;