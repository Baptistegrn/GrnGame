#pragma once

#include "wren.h"

#define KEY_SIZE 1024
#define MODULE_SIZE_MAX_NAME 512

void WriteFn(WrenVM *vm, const char *text);

void ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, int line, const char *msg);

WrenForeignMethodFn BindMethodFn(WrenVM *vm, const char *module, const char *className, bool isStatic,
                                 const char *signature);

WrenForeignClassMethods BindClassFn(WrenVM *vm, const char *module, const char *className);

WrenLoadModuleResult LoadModuleFn(WrenVM *vm, const char *name);

void InitBindingSystem();

// public

void RegisterClass(const char *module, const char *className, WrenForeignMethodFn allocateFn,
                   WrenFinalizerFn finalizeFn);

void RegisterMethod(const char *module, const char *className, bool isStatic, const char *signature,
                    WrenForeignMethodFn fn);
