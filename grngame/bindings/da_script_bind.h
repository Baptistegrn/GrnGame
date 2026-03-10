#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DaScriptVm DaScriptVm;

DaScriptVm* DaScriptEngineCreate(void);
void DaScriptEngineDestroy(DaScriptVm* self);
bool DaScriptEngineCompileScript(DaScriptVm* self, const char* script_name);
bool DaScriptEngineRunScript(DaScriptVm* self, const char* script_name, const char* entry);

#ifdef __cplusplus
}
#endif
