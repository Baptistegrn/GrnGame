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

void HotReloadInit(const char *folder);
void HotReloadDestroy();
void ProcessHotreloadQueue(void);

END_DECLARATIONS
