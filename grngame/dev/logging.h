#pragma once

#include "grngame/utils/c_cpp.h"
#include "haclog/haclog.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct AppInfo AppInfo;

#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_RED "\033[31m"
#define LOG_COLOR_GREEN "\033[32m"
#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_BLUE "\033[34m"
#define LOG_COLOR_MAGENTA "\033[35m"
#define LOG_COLOR_CYAN "\033[36m"

typedef enum
{
    LOG_TO_CONSOLE,
    LOG_TO_FILE,
} LogDestination;

typedef enum
{
    LOG_SEVERITY_DEBUG,
    LOG_SEVERITY_INFO,
    LOG_SEVERITY_WARNING,
    LOG_SEVERITY_ERROR,
    LOG_SEVERITY_CRITICAL
} LogSeverity;

bool LogInit(LogDestination log_destination);

#define LOG_DEBUG(fmt, ...) HACLOG_DEBUG(LOG_COLOR_CYAN fmt LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) HACLOG_INFO(LOG_COLOR_GREEN fmt LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) HACLOG_WARNING(LOG_COLOR_YELLOW fmt LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) HACLOG_ERROR(LOG_COLOR_RED fmt LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) HACLOG_FATAL(LOG_COLOR_MAGENTA fmt LOG_COLOR_RESET, ##__VA_ARGS__)

END_DECLARATIONS