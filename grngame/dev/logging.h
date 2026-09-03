#pragma once

#include "grngame/utils/c_cpp.h"

#ifndef GRNGAME_WASM

#include "haclog/haclog.h"

#else

#include <emscripten/emscripten.h>

#endif

#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct AppInfo AppInfo;

typedef enum
{
    LOG_TO_CONSOLE,
    LOG_TO_FILE,
} LogDestination;

bool LogInit(LogDestination log_destination);

#ifndef GRNGAME_WASM

#define LOG_DEBUG(fmt, ...) HACLOG_DEBUG(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) HACLOG_INFO(fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) HACLOG_WARNING(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) HACLOG_ERROR(fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) HACLOG_FATAL(fmt, ##__VA_ARGS__)

#else

#define LOG_DEBUG(fmt, ...) emscripten_log(EM_LOG_CONSOLE, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) emscripten_log(EM_LOG_CONSOLE, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) emscripten_log(EM_LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) emscripten_log(EM_LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) emscripten_log(EM_LOG_ERROR, fmt, ##__VA_ARGS__)

#endif

END_DECLARATIONS