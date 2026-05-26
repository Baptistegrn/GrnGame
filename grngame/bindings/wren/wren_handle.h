#pragma once

#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

bool WrenCallOnStart();
bool WrenCallOnUpdate(float delta);
bool WrenCallOnFixedUpdate(float delta);
bool WrenCallOnRender();
bool WrenCallOnDestroy();
void WrenFree();

bool WrenLoadMainHandles(const char *main_module_name);

END_DECLARATIONS