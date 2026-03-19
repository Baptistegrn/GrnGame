#pragma once
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef enum
{
    LOG_TO_CONSOLE,
    LOG_TO_FILE,
    LOG_TO_JSON,
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
void Log(LogSeverity log_severity, const char *format, ...);

#define LOG_DEBUG(fmt, ...) Log(LOG_SEVERITY_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Log(LOG_SEVERITY_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Log(LOG_SEVERITY_WARNING, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Log(LOG_SEVERITY_ERROR, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) Log(LOG_SEVERITY_CRITICAL, fmt, ##__VA_ARGS__)

END_DECLARATIONS