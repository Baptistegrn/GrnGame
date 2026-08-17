#pragma once
#include "grngame/utils/attributes.h"
#include <glib.h>
#include <glib/gthreadpool.h>

BEGIN_DECLARATIONS

typedef struct
{
    GThreadPool *pool;
    GMutex mutex;
    GCond cond;
    guint pending_jobs;
} ThreadManager;

// need to free
typedef void (*ThreadJobFunc)(void *data);

COLD ThreadManager ThreadManagerCreate(void);
COLD void ThreadManagerDestroy(ThreadManager *manager);

void ThreadManagerPush(ThreadJobFunc func, void *data);
void ThreadManagerWait();

END_DECLARATIONS
