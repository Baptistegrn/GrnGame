#pragma once

extern "C"
{
#include <wren.h>
}

WrenForeignClassMethods BindForeignClassCallbackSound(WrenVM *vm, const char *module, const char *class_name);
WrenForeignMethodFn BindForeignMethodCallbackSound(WrenVM *vm, const char *module, const char *class_name,
                                                   bool is_static, const char *signature);
