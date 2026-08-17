#include "thread.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include <stdlib.h>

typedef struct
{
    ThreadJobFunc func;
    void *data;
} ThreadJob;

static void ThreadPoolWorker(gpointer data, gpointer user_data)
{
    (void)user_data;
    ThreadManager *manager = &g_app.thread_manager;
    ThreadJob *job = (ThreadJob *)data;

    job->func(job->data);

    free(job);

    g_mutex_lock(&manager->mutex);

    manager->pending_jobs--;

    if (manager->pending_jobs == 0)
        g_cond_broadcast(&manager->cond);

    g_mutex_unlock(&manager->mutex);
}

COLD ThreadManager ThreadManagerCreate(void)
{
    guint num_threads = g_get_num_processors();

    if (num_threads < 1)
        num_threads = 1;

    ThreadManager manager = {0};

    g_mutex_init(&manager.mutex);
    g_cond_init(&manager.cond);

    GError *error = NULL;

    manager.pool = g_thread_pool_new(ThreadPoolWorker, NULL, (gint)num_threads, TRUE, &error);

    if (!manager.pool)
    {
        LOG_ERROR("Failed to create thread pool: %s", error->message);
        g_error_free(error);

        g_cond_clear(&manager.cond);
        g_mutex_clear(&manager.mutex);
    }
    else
    {
        LOG_DEBUG("ThreadManager: pool created with %u threads", num_threads);
    }

    return manager;
}

COLD void ThreadManagerDestroy(ThreadManager *manager)
{
    if (!manager || !manager->pool)
        return;

    g_thread_pool_free(manager->pool, FALSE, TRUE);

    manager->pool = NULL;

    g_cond_clear(&manager->cond);
    g_mutex_clear(&manager->mutex);
}

void ThreadManagerPush(ThreadJobFunc func, void *data)
{
    ThreadManager *manager = &g_app.thread_manager;

    ThreadJob *job = malloc(sizeof(ThreadJob));

    job->func = func;
    job->data = data;

    g_mutex_lock(&manager->mutex);
    manager->pending_jobs++;
    g_mutex_unlock(&manager->mutex);

    GError *error = NULL;

    if (!g_thread_pool_push(manager->pool, job, &error))
    {
        LOG_ERROR("ThreadManagerPush failed: %s", error->message);
        g_error_free(error);

        free(job);

        g_mutex_lock(&manager->mutex);
        manager->pending_jobs--;

        if (manager->pending_jobs == 0)
            g_cond_broadcast(&manager->cond);

        g_mutex_unlock(&manager->mutex);
    }
}

void ThreadManagerWait(void)
{
    ThreadManager *manager = &g_app.thread_manager;

    g_mutex_lock(&manager->mutex);

    while (manager->pending_jobs > 0)
        g_cond_wait(&manager->cond, &manager->mutex);

    g_mutex_unlock(&manager->mutex);
}
