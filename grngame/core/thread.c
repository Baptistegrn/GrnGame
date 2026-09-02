#include "thread.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

static int32 ThreadPoolWorker(void *user_data)
{
    haclog_thread_context_init();
    ThreadManager *manager = (ThreadManager *)user_data;
    while (1)
    {
        SDL_LockMutex(manager->mutex);
        while (manager->job_queue_head == NULL && !manager->shutdown)
        {
            SDL_WaitCondition(manager->queue_cond, manager->mutex);
        }
        if (manager->shutdown && manager->job_queue_head == NULL)
        {
            SDL_UnlockMutex(manager->mutex);
            break;
        }

        ThreadJobNode *job = manager->job_queue_head;
        if (job != NULL)
        {
            manager->job_queue_head = job->next;
            if (manager->job_queue_head == NULL)
            {
                manager->job_queue_tail = NULL;
            }
        }

        SDL_UnlockMutex(manager->mutex);
        if (job != NULL)
        {
            job->func(job->data);
            free(job);
            SDL_LockMutex(manager->mutex);
            manager->pending_jobs--;
            if (manager->pending_jobs == 0)
            {
                SDL_BroadcastCondition(manager->wait_cond);
            }

            SDL_UnlockMutex(manager->mutex);
        }
    }
    haclog_thread_context_cleanup();
    return 0;
}

COLD void ThreadManagerCreate(void)
{
    int32 num_threads = SDL_GetNumLogicalCPUCores();

    if (num_threads < 1)
        num_threads = 1;

    ThreadManager manager = {0};

    manager.mutex = SDL_CreateMutex();
    manager.queue_cond = SDL_CreateCondition();
    manager.wait_cond = SDL_CreateCondition();
    manager.shutdown = false;
    manager.num_workers = num_threads;

    manager.workers = malloc(sizeof(SDL_Thread *) * num_threads);

    g_app.thread_manager = manager;

    for (int32 i = 0; i < num_threads; i++)
    {

        g_app.thread_manager.workers[i] = SDL_CreateThread(ThreadPoolWorker, "", &g_app.thread_manager);

        if (!g_app.thread_manager.workers[i])
        {
            LOG_ERROR("Failed to create thread %d: %s", i, SDL_GetError());
        }
    }

    LOG_DEBUG("ThreadManager: pool created with %d threads", num_threads);
}

COLD void ThreadManagerDestroy(ThreadManager *manager)
{
    if (!manager || !manager->workers)
        return;

    SDL_LockMutex(manager->mutex);
    manager->shutdown = true;
    SDL_BroadcastCondition(manager->queue_cond);
    SDL_UnlockMutex(manager->mutex);

    for (int32 i = 0; i < manager->num_workers; i++)
    {
        if (manager->workers[i])
        {
            SDL_WaitThread(manager->workers[i], NULL);
        }
    }

    free(manager->workers);
    manager->workers = NULL;

    ThreadJobNode *current = manager->job_queue_head;
    while (current != NULL)
    {
        ThreadJobNode *next = current->next;
        free(current);
        current = next;
    }

    manager->job_queue_head = NULL;
    manager->job_queue_tail = NULL;

    SDL_DestroyCondition(manager->wait_cond);
    SDL_DestroyCondition(manager->queue_cond);
    SDL_DestroyMutex(manager->mutex);
}

void ThreadManagerPush(ThreadJobFunc func, void *data)
{
    ThreadManager *manager = &g_app.thread_manager;

    ThreadJobNode *job = malloc(sizeof(ThreadJobNode));

    job->func = func;
    job->data = data;
    job->next = NULL;

    SDL_LockMutex(manager->mutex);

    manager->pending_jobs++;

    if (manager->job_queue_tail == NULL)
    {
        manager->job_queue_head = job;
        manager->job_queue_tail = job;
    }
    else
    {
        manager->job_queue_tail->next = job;
        manager->job_queue_tail = job;
    }

    SDL_SignalCondition(manager->queue_cond);

    SDL_UnlockMutex(manager->mutex);
}

void ThreadManagerWait(void)
{
    ThreadManager *manager = &g_app.thread_manager;

    SDL_LockMutex(manager->mutex);

    while (manager->pending_jobs > 0)
    {
        SDL_WaitCondition(manager->wait_cond, manager->mutex);
    }

    SDL_UnlockMutex(manager->mutex);
}
