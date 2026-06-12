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

bool ReloadWrenScript(const char *filename);

bool WrenEarlyInit(void);
bool WrenLateInit(void);

END_DECLARATIONS