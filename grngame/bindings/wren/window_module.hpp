#pragma once

extern "C"
{
#include <wren.h>
}

WrenForeignClassMethods BindForeignClassCallbackWindow(WrenVM *vm, const char *module, const char *class_name);
WrenForeignMethodFn BindForeignMethodCallbackWindow(WrenVM *vm, const char *module, const char *class_name,
                                                    bool is_static, const char *signature);