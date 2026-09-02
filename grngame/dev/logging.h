#pragma once

#include "grngame/utils/c_cpp.h"
#include "haclog/haclog.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct AppInfo AppInfo;

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

#define LOG_DEBUG(fmt, ...) HACLOG_DEBUG(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) HACLOG_INFO(fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) HACLOG_WARNING(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) HACLOG_ERROR(fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) HACLOG_FATAL(fmt, ##__VA_ARGS__)

END_DECLARATIONS