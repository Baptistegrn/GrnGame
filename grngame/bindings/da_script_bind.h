#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// opaque pointer to a DaScriptEngine C++ class
typedef struct DaScriptVm DaScriptVm;

/// Creates a new dascript vm
DaScriptVm* DaScriptEngineCreate(void);

/// Destroys the dascript vm
void DaScriptEngineDestroy(DaScriptVm* self);

/// Tries to compile the script named script_name
/// It will look in the scripts folder for a script named script_name.das
/// Works with scripts in subfolders
/// Returns wether or not it succeeded, and logs the errors
bool DaScriptEngineCompileScript(DaScriptVm* self, const char* script_name);

/// Tries to run the script named script_name
/// Returns wether or not it succeeded, and logs the errors
bool DaScriptEngineRunScript(DaScriptVm* self, const char* script_name, const char* entry);

#ifdef __cplusplus
}
#endif
