#pragma once
#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef void (*ThreadJobFunc)(void *);

typedef struct ThreadJobNode
{
    ThreadJobFunc func;
    void *data;
    struct ThreadJobNode *next;
} ThreadJobNode;

typedef struct
{
    SDL_Thread **workers;
    int32 num_workers;

    SDL_Mutex *mutex;
    SDL_Condition *queue_cond;
    SDL_Condition *wait_cond;

    ThreadJobNode *job_queue_head;
    ThreadJobNode *job_queue_tail;

    int32 pending_jobs;
    bool shutdown;
} ThreadManager;

void ThreadManagerCreate(void);
void ThreadManagerDestroy(ThreadManager *manager);

void ThreadManagerPush(ThreadJobFunc func, void *data);
void ThreadManagerWait();

END_DECLARATIONS
