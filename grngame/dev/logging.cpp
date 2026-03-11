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
#include <stdarg.h>

static LogSeverity LogSeverityForBuildType();
static quill::LogLevel LogSeverityToLogLevel(LogSeverity log_severity);
static quill::Logger *s_logger = nullptr;

extern "C"
{
    bool LogInit(LogDestination log_destination)
    {
        quill::Backend::start();

        switch (log_destination)
        {
        case LOG_TO_CONSOLE: {
            auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1");
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

    void Log(LogSeverity log_severity, const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        int size = vsnprintf(nullptr, 0, format, args);
        va_end(args);

        if (size < 0)
            return;

        auto buf = std::vector<char>(size + 1);
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
    return LOG_SEVERITY_WARNING;
#endif
}
