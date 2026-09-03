#ifndef GRNGAME_WASM
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/clear.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


int WriteMeta(struct haclog_handler *handler, haclog_meta_info_t *meta)
{
    const char *level = haclog_level_to_str(meta->loc->level);

    char filename[256];
    haclog_path_basename(meta->loc->file, filename, sizeof(filename));

    struct tm t;
    gmtime_r(&meta->ts.tv_sec, &t);

    return handler->writev(handler, "%s|%s:%u|%s|%llu - ", level, filename, (unsigned int)meta->loc->line,
                           meta->loc->func, (unsigned long long)meta->tid);
}

static bool add_console_handler()
{
    static haclog_console_handler_t handler;
    CLEAR(handler, 0);
    if (haclog_console_handler_init(&handler, true) != 0)
    {
        return false;
    }
    haclog_handler_set_fn_write_meta((haclog_handler_t *)&handler, WriteMeta);
#ifdef GRNGAME_RELEASE
    haclog_handler_set_level((haclog_handler_t *)&handler, HACLOG_LEVEL_INFO);
#else
    haclog_handler_set_level((haclog_handler_t *)&handler, HACLOG_LEVEL_DEBUG);
#endif
    haclog_context_add_handler((haclog_handler_t *)&handler);
    return true;
}

static bool add_file_handler()
{
    static haclog_file_handler_t handler;
    CLEAR(handler, 0);
    char *path = PathFromExecutableDirectory("grngame.log");
    if (haclog_file_handler_init(&handler, path, "w") != 0)
    {
        free(path);
        return false;
    }
    haclog_handler_set_fn_write_meta((haclog_handler_t *)&handler, WriteMeta);
#ifdef GRNGAME_RELEASE
    haclog_handler_set_level((haclog_handler_t *)&handler, HACLOG_LEVEL_INFO);
#else
    haclog_handler_set_level((haclog_handler_t *)&handler, HACLOG_LEVEL_DEBUG);
#endif
    haclog_context_add_handler((haclog_handler_t *)&handler);
    free(path);
    return true;
}

bool LogInit(LogDestination log_destination)
{
    bool ok;
    if (log_destination == LOG_TO_CONSOLE)
    {
        ok = add_console_handler();
    }
    else
    {
        ok = add_file_handler();
    }

    if (!ok)
    {
        return false;
    }

    haclog_backend_run();

    haclog_thread_context_init();

    return true;
}

#else

#include "grngame/dev/logging.h"

bool LogInit(LogDestination log_destination)
{
    (void)log_destination;
    return true;
}

#endif