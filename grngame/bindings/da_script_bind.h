#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct DaScriptManager DaScriptManager;

    DaScriptManager *DaScriptManagerNew();
    void DaScriptManagerDelete(DaScriptManager *manager);
    bool DaScriptManagerInitialize(DaScriptManager *manager, const char *main_script_name);

    bool DaScriptManagerCallOnStart(DaScriptManager *manager);
    bool DaScriptManagerCallOnUpdate(DaScriptManager *manager, float delta);
    bool DaScriptManagerCallOnFixedUpdate(DaScriptManager *manager, float delta);
    bool DaScriptManagerCallOnRender(DaScriptManager *manager);
    bool DaScriptManagerCallOnDestroy(DaScriptManager *manager);

#ifdef __cplusplus
}
#endif