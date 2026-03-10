#include "da_script_bind.h"

#include "da_script_engine.hpp"

extern "C" {

    DaScriptManager* DaScriptManagerNew()
    {
        return reinterpret_cast<DaScriptManager*>(new DaScriptEngine());
    }

    void DaScriptManagerDelete(DaScriptManager* manager)
    {
        delete reinterpret_cast<DaScriptEngine*>(manager);
    }

    bool DaScriptManagerInitialize(DaScriptManager* manager, const char* main_script_name)
    {
        return reinterpret_cast<DaScriptEngine*>(manager)->Init(main_script_name);
    }

    bool DaScriptManagerCallOnStart(DaScriptManager* manager)
    {
        return reinterpret_cast<DaScriptEngine*>(manager)->CallOnStart();
    }

    bool DaScriptManagerCallOnUpdate(DaScriptManager* manager, float delta)
    {
        return reinterpret_cast<DaScriptEngine*>(manager)->CallOnUpdate(delta);
    }

    bool DaScriptManagerCallOnFixedUpdate(DaScriptManager* manager, float delta)
    {
        return reinterpret_cast<DaScriptEngine*>(manager)->CallOnFixedUpdate(delta);
    }

    bool DaScriptManagerCallOnRender(DaScriptManager* manager)
    {
        return reinterpret_cast<DaScriptEngine*>(manager)->CallOnRender();
    }

    bool DaScriptManagerCallOnDestroy(DaScriptManager* manager)
    {
        return reinterpret_cast<DaScriptEngine*>(manager)->CallOnDestroy();
    }

} // extern "C"