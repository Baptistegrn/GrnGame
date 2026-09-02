#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/clear.h"
#include <stdbool.h>
#include <string.h>

static bool add_console_handler()
{
    static haclog_console_handler_t handler;
    CLEAR(handler, 0);
    if (haclog_console_handler_init(&handler, true) != 0)
    {
        return false;
    }
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
    // dont need to free
    char *path = PathFromExecutableDirectory("grngame.log");
    if (haclog_file_handler_init(&handler, path, "w") != 0)
    {
        return false;
    }
#ifdef GRNGAME_RELEASE
    haclog_handler_set_level((haclog_handler_t *)&handler, HACLOG_LEVEL_INFO);
#else
    haclog_handler_set_level((haclog_handler_t *)&handler, HACLOG_LEVEL_DEBUG);
#endif
    haclog_context_add_handler((haclog_handler_t *)&handler);
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