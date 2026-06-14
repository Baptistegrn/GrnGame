#ifndef WASM

#include "logging.h"
#include "grngame/dev/logging.h"

#define QUILL_DISABLE_NON_PREFIXED_MACROS
#include "grngame/utils/attributes.h"

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>
#include <quill/sinks/JsonSink.h>

#include <cstdio>
#include <stdarg.h>
#include <vector>

static quill::Logger *s_logger = nullptr;
static LogDestination s_current_destination;
static bool s_logs_enabled = true;

static LogSeverity LogSeverityForBuildType();
static quill::LogLevel LogSeverityToLogLevel(LogSeverity log_severity);

extern "C"
{
    void LogSetEnabled(bool enabled)
    {
        s_logs_enabled = enabled;
    }

    bool LogIsEnabled()
    {
        return s_logs_enabled;
    }

    bool LogSetDestination(LogDestination log_destination)
    {
        if (s_current_destination == log_destination && s_logger)
            return true;

        return LogInit(log_destination);
    }

    void LogSetLevel(LogSeverity severity)
    {
        if (s_logger)
            s_logger->set_log_level(LogSeverityToLogLevel(severity));
    }
}

extern "C"
{
    bool LogInit(LogDestination log_destination)
    {
        quill::Backend::start();

        s_current_destination = log_destination;
        s_logger = nullptr;

        switch (log_destination)
        {
        case LOG_TO_CONSOLE: {
            auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_console");

            s_logger = quill::Frontend::create_or_get_logger("grngame", std::move(console_sink));
            break;
        }

        case LOG_TO_FILE: {
            auto cfg = quill::FileSinkConfig();
            cfg.set_open_mode('w');
            cfg.set_filename_append_option(quill::FilenameAppendOption::StartDateTime);

            auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>("grngame.log", std::move(cfg),
                                                                                  quill::FileEventNotifier());

            s_logger = quill::Frontend::create_or_get_logger("grngame", std::move(file_sink));
            break;
        }

        case LOG_TO_JSON: {
            auto cfg = quill::FileSinkConfig();
            cfg.set_open_mode('w');
            cfg.set_filename_append_option(quill::FilenameAppendOption::None);

            auto json_sink = quill::Frontend::create_or_get_sink<quill::JsonFileSink>("grngame.log", std::move(cfg),
                                                                                      quill::FileEventNotifier());

            s_logger = quill::Frontend::create_or_get_logger("grngame", std::move(json_sink));
            break;
        }
        }

        if (UNLIKELY(!s_logger))
            return false;

        s_logger->set_log_level(LogSeverityToLogLevel(LogSeverityForBuildType()));

        return true;
    }
}

extern "C"
{
    void Log(LogSeverity log_severity, const char *format, ...)
    {
        if (!s_logs_enabled)
            return;

        va_list args;
        va_start(args, format);

        int size = vsnprintf(nullptr, 0, format, args);
        va_end(args);

        if (size < 0)
            return;

        std::vector<char> buf(size + 1);

        va_start(args, format);
        vsnprintf(buf.data(), buf.size(), format, args);
        va_end(args);

        if (UNLIKELY(!s_logger))
        {
            fprintf(stderr, "%s\n", buf.data());
            fflush(stderr);
            return;
        }

        QUILL_LOG_DYNAMIC(s_logger, LogSeverityToLogLevel(log_severity), "{}", buf.data());
    }
}

static quill::LogLevel LogSeverityToLogLevel(LogSeverity log_severity)
{
    switch (log_severity)
    {
    case LOG_SEVERITY_DEBUG:
        return quill::LogLevel::Debug;
    case LOG_SEVERITY_INFO:
        return quill::LogLevel::Info;
    case LOG_SEVERITY_WARNING:
        return quill::LogLevel::Warning;
    case LOG_SEVERITY_ERROR:
        return quill::LogLevel::Error;
    case LOG_SEVERITY_CRITICAL:
        return quill::LogLevel::Critical;
    default:
        UNREACHABLE();
    }
}

static LogSeverity LogSeverityForBuildType()
{
#ifdef GRNGAME_DEBUG
    return LOG_SEVERITY_DEBUG;
#else
    return LOG_SEVERITY_DEBUG;
#endif
}

#endif

#ifdef WASM

#include "logging.h"
#include <emscripten/emscripten.h>
#include <stdarg.h>
#include <stdio.h>
#include <vector>

bool LogInit(LogDestination log_destination)
{
    return true;
}

void Log(LogSeverity log_severity, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int size = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (size < 0)
        return;

    std::vector<char> buf(size + 1);

    va_start(args, format);
    vsnprintf(buf.data(), buf.size(), format, args);
    va_end(args);

    switch (log_severity)
    {
    case LOG_SEVERITY_DEBUG:
        EM_ASM({ console.debug(UTF8ToString($0)); }, buf.data());
        break;

    case LOG_SEVERITY_INFO:
        EM_ASM({ console.info(UTF8ToString($0)); }, buf.data());
        break;

    case LOG_SEVERITY_WARNING:
        EM_ASM({ console.warn(UTF8ToString($0)); }, buf.data());
        break;

    case LOG_SEVERITY_ERROR:
    case LOG_SEVERITY_CRITICAL:
        EM_ASM({ console.error(UTF8ToString($0)); }, buf.data());
        break;
    }
}

static void ApplyEnableLogs(AppInfo *info)
{
    LogSetEnabled(info->enable_logs);
}

static void ApplyLogDestination(AppInfo *info)
{
    LogSetDestination(info->log_destination);
}

#endif