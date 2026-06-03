#ifndef WASM

#pragma once

#include "grngame/utils/c_cpp.h"

typedef enum
{
    ADD,
    DELETE,
    MODIFIED,
    MOVED,
} Action;

typedef struct
{
    const char *old_file;
    const char *new_file;
    Action action;

} HotreloadQueueElement;

BEGIN_DECLARATIONS
void StartAssetHotReload(const char *directory, bool recursive);
void ProcessHotreloadQueue();
void HotReloadInitQueue();
END_DECLARATIONS

#endif