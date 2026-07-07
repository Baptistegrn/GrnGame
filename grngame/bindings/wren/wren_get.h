#pragma once
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>


BEGIN_DECLARATIONS

bool WrenGetVariable(const char *module, const char *variable);
const char *WrenGetString(const char *module, const char *variable, const char *field);
float64 WrenGetDouble(const char *module, const char *variable, const char *field);
int32 WrenGetInt(const char *module, const char *variable, const char *field);
bool WrenGetBool(const char *module, const char *variable, const char *field);
float64 WrenGetListDouble(const char *module, const char *variable, const char *field, int32 index);
int32 WrenGetListCount(const char *module, const char *variable, const char *field);
const char *WrenGetListString(const char *module, const char *variable, const char *field, int32 index);

END_DECLARATIONS