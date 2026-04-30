#pragma once

extern "C"
{
#include <wren.h>
}

struct WrenVM;
typedef void (*WrenForeignMethodFn)(WrenVM *vm);

WrenForeignMethodFn BindForeignMethodCallbackFile(WrenVM *vm, const char *module, const char *class_name,
                                                  bool is_static, const char *signature);
WrenForeignClassMethods BindForeignClassCallbackFile(WrenVM *vm, const char *module, const char *class_name);