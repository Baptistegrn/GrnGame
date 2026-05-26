#pragma once

#include "grngame/utils/c_cpp.h"
#include "wren.h"

BEGIN_DECLARATIONS

typedef struct
{
    WrenConfiguration config;
    WrenVM *vm;
    WrenHandle *main_class;
    WrenHandle *on_start;
    WrenHandle *on_update;
    WrenHandle *on_fixed_update;
    WrenHandle *on_render;
    WrenHandle *on_destroy;
} WrenManager;

void WrenInit();

void WrenSetWriteFn(WrenWriteFn writeFn);

void WrenSetErrorFn(WrenErrorFn errorFn);

void WrenSetBindMethodFn(WrenBindForeignMethodFn bindMethodFn);

void WrenSetBindClassFn(WrenBindForeignClassFn bindClassFn);

void WrenSetLoadModuleFn(WrenLoadModuleFn loadModuleFn);

void WrenStartVM();

bool WrenInterpret(const char *filename);

END_DECLARATIONS