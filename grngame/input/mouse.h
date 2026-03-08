#pragma once
#include "../math/types.h"
#include "cglm/types-struct.h"
#include <stdbool.h>

typedef struct
{
    int32 x, y;
    bool left_pressed;
    bool left_just_pressed;
    bool right_pressed;
    bool right_just_pressed;
    int8 scroll_x;
    int8 scroll_y;
} Mouse;

// intern
vec2s GetMousePosition();

bool MouseLeftPressed();
bool MouseLeftJustPressed();
bool MouseRightPressed();
bool MouseRightJustPressed();
int32 MouseX();
int32 MouseY();
int8 MouseScrollX();
int8 MouseScrollY();