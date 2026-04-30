#pragma once
#include "../math/types.h"
#include "cglm/types-struct.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct
{
    int32 x, y;
    bool left_pressed;
    bool left_just_pressed;
    bool right_pressed;
    bool right_just_pressed;
    bool left_just_released;
    bool right_just_released;
    float32 scroll_x;
    float32 scroll_y;
} Mouse;

vec2s GetMousePosition();

bool MouseLeftPressed();
bool MouseLeftJustPressed();
bool MouseLeftJustReleased();

bool MouseRightPressed();
bool MouseRightJustPressed();
bool MouseRightJustReleased();

int32 MouseX();
int32 MouseY();
float32 MouseScrollX();
float32 MouseScrollY();
void MoveMouse(int16 x, int16 y);
bool ShowCursor();
bool HideCursor();

END_DECLARATIONS