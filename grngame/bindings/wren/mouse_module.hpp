#pragma once

extern "C"
{
#include <wren.h>
}

WrenForeignClassMethods BindForeignClassCallbackMouse(WrenVM *vm, const char *module, const char *class_name);
WrenForeignMethodFn BindForeignMethodCallbackMouse(WrenVM *vm, const char *module, const char *class_name,
                                                   bool is_static, const char *signature);