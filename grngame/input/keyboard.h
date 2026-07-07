#pragma once
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS;

bool KeyPressed(int32 key);
bool KeyJustPressed(int32 key);
bool KeyJustReleased(int32 key);

END_DECLARATIONS;