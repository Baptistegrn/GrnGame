#pragma once
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

bool WrenGetVariable(const char *module, const char *variable);

const char *WrenGetString(const char *module, const char *variable, const char *field);
double WrenGetDouble(const char *module, const char *variable, const char *field);
int WrenGetInt(const char *module, const char *variable, const char *field);
bool WrenGetBool(const char *module, const char *variable, const char *field);
double WrenGetListDouble(const char *module, const char *variable, const char *field, int index);
int WrenGetListCount(const char *module, const char *variable, const char *field);

END_DECLARATIONS