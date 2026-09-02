#if defined(GRNGAME_HOT_RELOAD_ENABLE)
#pragma once

#include "grngame/utils/c_cpp.h"
#include "stdbool.h"

typedef enum
{
    ADD,
    DELETE_,
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
void HotReloadDestroyQueue();
END_DECLARATIONS

#endif
