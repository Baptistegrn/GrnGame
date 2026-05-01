#pragma once

extern "C"
{
#include <wren.h>
}

WrenForeignClassMethods BindForeignClassCallbackDb(WrenVM *vm, const char *module, const char *class_name);
WrenForeignMethodFn BindForeignMethodCallbackDb(WrenVM *vm, const char *module, const char *class_name, bool is_static,
                                                const char *signature);