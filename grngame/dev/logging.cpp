#include "logging.h"
#include "grngame/core/app.h"

#include <cstdarg>
#include <cstdio>
#include <vector>

static LogDestination s_current_destination = LOG_TO_CONSOLE;
static LogSeverity s_current_severity = LOG_SEVERITY_INFO;

static LogSeverity LogSeverityForBuildType()
{
#ifdef GRNGAME_DEBUG
    return LOG_SEVERITY_DEBUG;
#else
    return LOG_SEVERITY_INFO;
#endif
}

#ifndef WASM

#include "grngame/dev/logging.h"

#define QUILL_DISABLE_NON_PREFIXED_MACROS
#include "grngame/utils/attributes.h"

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>

#include <chrono>
#include <string>

static quill::Logger *s_logger = nullptr;
static quill::Logger *s_logger_console = nullptr;
static quill::Logger *s_logger_file = nullptr;
static quill::Logger *s_logger_json = nullptr;

static quill::LogLevel LogSeverityToLogLevel(LogSeverity log_severity);
static const char *LogSeverityToString(LogSeverity log_severity);

extern "C"
{

    bool LogSetDestination(LogDestination log_destination)
    {
        switch (log_destination)
        {
        case LOG_TO_CONSOLE:
            s_logger = s_logger_console;
            break;
        case LOG_TO_FILE:
            s_logger = s_logger_file;
            break;
        case LOG_TO_JSON:
            s_logger = s_logger_json;
            break;
        default:
            return false;
        }

        if (UNLIKELY(!s_logger))
            return false;

        s_current_destination = log_destination;
        return true;
    }

    void LogSetLevel(LogSeverity severity)
    {
        s_current_severity = severity;
        const auto level = LogSeverityToLogLevel(severity);
        if (s_logger_console)
            s_logger_console->set_log_level(level);
        if (s_logger_file)
            s_logger_file->set_log_level(level);
        if (s_logger_json)
            s_logger_json->set_log_level(level);
    }

    bool LogInit(LogDestination log_destination)
    {
        quill::Backend::start();

        {
            auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_console");
            s_logger_console = quill::Frontend::create_or_get_logger("grngame_console", console_sink);
        }

        {
            quill::FileSinkConfig cfg;
            cfg.set_open_mode('w');
            cfg.set_filename_append_option(quill::FilenameAppendOption::None);

            auto file_sink =
                quill::Frontend::create_or_get_sink<quill::FileSink>("grngame.log", cfg, quill::FileEventNotifier{});

            s_logger_file = quill::Frontend::create_or_get_logger("grngame_file", file_sink);
        }

        {
            quill::FileSinkConfig cfg;
            cfg.set_open_mode('w');
            cfg.set_filename_append_option(quill::FilenameAppendOption::None);

            auto file_sink =
                quill::Frontend::create_or_get_sink<quill::FileSink>("grngame.log", cfg, quill::FileEventNotifier{});

            s_logger_json = quill::Frontend::create_or_get_logger("grngame_json", file_sink,
                                                                  quill::PatternFormatterOptions{"%(message)"});
        }

        if (UNLIKELY(!s_logger_console || !s_logger_file || !s_logger_json))
            return false;

        const LogSeverity default_severity = LogSeverityForBuildType();
        s_current_severity = default_severity;
        s_logger_console->set_log_level(LogSeverityToLogLevel(default_severity));
        s_logger_file->set_log_level(LogSeverityToLogLevel(default_severity));
        s_logger_json->set_log_level(LogSeverityToLogLevel(default_severity));

        return LogSetDestination(log_destination);
    }

} // extern "C"

void Log(LogSeverity log_severity, const char *format, ...)
{
    if (!g_app.info.enable_logs)
        return;

    va_list args;
    va_start(args, format);

    int size = vsnprintf(nullptr, 0, format, args);

    va_end(args);

    if (size < 0)
        return;

    std::vector<char> buf(static_cast<size_t>(size) + 1);

    va_start(args, format);
    vsnprintf(buf.data(), buf.size(), format, args);
    va_end(args);

    if (UNLIKELY(!s_logger))
    {
        fprintf(stderr, "%s\n", buf.data());
        fflush(stderr);
        return;
    }

    if (s_current_destination == LOG_TO_JSON)
    {
        std::string msg(buf.data());

        std::string escaped;
        escaped.reserve(msg.size());

        for (char c : msg)
        {
            switch (c)
            {
            case '\\':
                escaped += "\\\\";
                break;
            case '"':
                escaped += "\\\"";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += c;
                break;
            }
        }

        auto now = std::chrono::system_clock::now();

        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

        std::string json_line = std::string("{\"timestamp\":\"") + std::to_string(ns) + "\",\"log_level\":\"" +
                                LogSeverityToString(log_severity) + "\",\"message\":\"" + escaped + "\"}";

        QUILL_LOG_DYNAMIC(s_logger, LogSeverityToLogLevel(log_severity), "{}", json_line);
    }
    else
    {
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
    }

    return quill::LogLevel::Info;
}

static const char *LogSeverityToString(LogSeverity log_severity)
{
    switch (log_severity)
    {
    case LOG_SEVERITY_DEBUG:
        return "DEBUG";

    case LOG_SEVERITY_INFO:
        return "INFO";

    case LOG_SEVERITY_WARNING:
        return "WARNING";

    case LOG_SEVERITY_ERROR:
        return "ERROR";

    case LOG_SEVERITY_CRITICAL:
        return "CRITICAL";

    default:
        return "UNKNOWN";
    }
}

#endif // !WASM

#ifdef WASM

#include <emscripten/emscripten.h>

extern "C"
{

    bool LogInit(LogDestination)
    {
        // wasm only console work
        s_current_destination = LOG_TO_CONSOLE;
        s_current_severity = LogSeverityForBuildType();
        g_app.info.enable_logs = true;

        return true;
    }

    bool LogSetDestination(LogDestination)
    {

        s_current_destination = LOG_TO_CONSOLE;
        return true;
    }

    void LogSetLevel(LogSeverity severity)
    {
        s_current_severity = severity;
    }

} // extern "C"

void Log(LogSeverity log_severity, const char *format, ...)
{
    if (log_severity < s_current_severity)
        return;

    va_list args;
    va_start(args, format);

    int size = vsnprintf(nullptr, 0, format, args);

    va_end(args);

    if (size < 0)
        return;

    std::vector<char> buf(static_cast<size_t>(size) + 1);

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

#endif // WASM

static void ApplyEnableLogs(AppInfo *info)
{
#ifdef WASM
    // log enable on wasm
    (void)info;
    g_app.info.enable_logs = true;
#else
    g_app.info.enable_logs = info->enable_logs;
#endif
}

static void ApplyLogDestination(AppInfo *info)
{
    LogSetDestination(info->log_destination);
}

bool LogGetEnable(void)
{
    return g_app.info.enable_logs;
}

void LogInfoSetEnable(bool enable)
{
#ifdef WASM
    // impossible to desativate log in wasm
    (void)enable;
    g_app.info.enable_logs = true;
#else
    g_app.info.enable_logs = enable;
#endif
}

void LogInfoSetDestination(LogDestination destination)
{
    g_app.info.log_destination = destination;
}

void LogApplyConfig(AppInfo *app_info)
{
    ApplyLogDestination(app_info);
    ApplyEnableLogs(app_info);
}

extern "C"
{

    LogSeverity LogGetLevel(void)
    {
        return s_current_severity;
    }

    LogDestination LogGetDestination(void)
    {
        return s_current_destination;
    }
}