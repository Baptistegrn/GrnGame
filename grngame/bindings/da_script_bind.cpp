#include "da_script_bind.h"
#include "da_script_engine.hpp"


extern "C" {
DaScriptVm* DaScriptEngineCreate(void)
{
    return reinterpret_cast<DaScriptVm*>(new DaScriptEngine());
}

void DaScriptEngineDestroy(DaScriptVm* self)
{
    delete reinterpret_cast<DaScriptEngine*>(self);
}

bool DaScriptEngineCompileScript(DaScriptVm* self, const char* script_name)
{
    return reinterpret_cast<DaScriptEngine*>(self)->CompileScript(script_name);
}

bool DaScriptEngineRunScript(DaScriptVm* self, const char* script_name, const char* entry)
{
    return reinterpret_cast<DaScriptEngine*>(self)->RunScript(script_name, entry);
}
} // extern "C"
