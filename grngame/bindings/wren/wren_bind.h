#pragma once

#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct WrenManager WrenManager;

WrenManager *WrenManagerNew();
void WrenManagerDelete(WrenManager *manager);
bool WrenManagerInitialize(WrenManager *manager, const char *main_script_name);

bool WrenManagerCallOnStart(WrenManager *manager);
bool WrenManagerCallOnUpdate(WrenManager *manager, float delta);
bool WrenManagerCallOnFixedUpdate(WrenManager *manager, float delta);
bool WrenManagerCallOnRender(WrenManager *manager);
bool WrenManagerCallOnDestroy(WrenManager *manager);

END_DECLARATIONS
