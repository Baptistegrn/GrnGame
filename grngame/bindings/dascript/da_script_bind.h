#pragma once
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct DaScriptManager DaScriptManager;

DaScriptManager *DaScriptManagerNew();
void DaScriptManagerDelete(DaScriptManager *manager);
bool DaScriptManagerInitialize(DaScriptManager *manager, const char *main_script_name);

bool DaScriptManagerCallOnStart(DaScriptManager *manager);
bool DaScriptManagerCallOnUpdate(DaScriptManager *manager, float delta);
bool DaScriptManagerCallOnFixedUpdate(DaScriptManager *manager, float delta);
bool DaScriptManagerCallOnRender(DaScriptManager *manager);
bool DaScriptManagerCallOnDestroy(DaScriptManager *manager);

END_DECLARATIONS