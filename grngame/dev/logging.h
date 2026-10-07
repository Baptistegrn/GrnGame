#pragma once

#include "grngame/utils/c_cpp.h"

#include <stdbool.h>

#if defined(GRNGAME_DESKTOP)

#include "haclog/haclog.h"

#define LOG_DEBUG(fmt, ...) HACLOG_DEBUG(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) HACLOG_INFO(fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) HACLOG_WARNING(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) HACLOG_ERROR(fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) HACLOG_FATAL(fmt, ##__VA_ARGS__)

#elif defined(GRNGAME_ANDROID)

#include <android/log.h>

#define LOG_DEBUG(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "GrnGame", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "GrnGame", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) __android_log_print(ANDROID_LOG_WARN, "GrnGame", fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, "GrnGame", fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) __android_log_print(ANDROID_LOG_FATAL, "GrnGame", fmt, ##__VA_ARGS__)

#elif defined(GRNGAME_IOS)

#include <os/log.h>

#define LOG_DEBUG(fmt, ...) os_log_debug(OS_LOG_DEFAULT, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) os_log_info(OS_LOG_DEFAULT, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) os_log(OS_LOG_DEFAULT, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) os_log_error(OS_LOG_DEFAULT, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) os_log_fault(OS_LOG_DEFAULT, fmt, ##__VA_ARGS__)

#elif defined(GRNGAME_WASM)

#include <emscripten/emscripten.h>

#define LOG_DEBUG(fmt, ...) emscripten_log(EM_LOG_CONSOLE, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) emscripten_log(EM_LOG_CONSOLE, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) emscripten_log(EM_LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) emscripten_log(EM_LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) emscripten_log(EM_LOG_ERROR, fmt, ##__VA_ARGS__)

#endif

BEGIN_DECLARATIONS

typedef struct AppInfo AppInfo;

typedef enum
{
    LOG_TO_CONSOLE,
    LOG_TO_FILE,
} LogDestination;

bool LogInit(LogDestination log_destination);
void LogDestroy(void);

END_DECLARATIONS