#pragma once

extern "C"
{
#include <wren.h>
}

WrenForeignClassMethods BindForeignClassCallbackRenderer(WrenVM *vm, const char *module, const char *class_name);
WrenForeignMethodFn BindForeignMethodCallbackRenderer(WrenVM *vm, const char *module, const char *class_name,
                                                      bool is_static, const char *signature);