#include "logging.h"
#include "quill/LogFunctions.h"
#include "quill/core/LogLevel.h"
#include <cstdarg>
#include <format>
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>
#include <utility>


static quill::Logger* g_logger = nullptr;

quill::LogLevel niveau_log_vers_quill_log_level(NiveauLog niveau_log) {
    switch (niveau_log) {
    case NiveauLogDebug:
        return quill::LogLevel::Debug;
    case NiveauLogInfo:
        return quill::LogLevel::Info;
    case NiveauLogAvertissement:
        return quill::LogLevel::Warning;
    case NiveauLogErreur:
        return quill::LogLevel::Error;
    default:
        std::unreachable();
    }
}

void initialiser_logging(DestinationLog destination_log, const char *nom_fichier, NiveauLog niveau_log) {
    if (g_logger) {
        delete g_logger;
    } else {
        quill::Backend::start();
    }


    if (destination_log == DestinationLogFichier) {
        auto file_sink_config = quill::FileSinkConfig();
        file_sink_config.set_open_mode('w');
        file_sink_config.set_filename_append_option(quill::FilenameAppendOption::StartDateTime);

        auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>(nom_fichier, file_sink_config);
        g_logger = quill::Frontend::create_or_get_logger("root", std::move(file_sink));
    } else if (destination_log == DestinationLogConsole) {
        g_logger = quill::Frontend::create_or_get_logger("root", quill::Frontend::create_or_get_sink<quill::ConsoleSink>("console_sink"));
    }

    g_logger->set_log_level(niveau_log_vers_quill_log_level(niveau_log));
    
    log_fmt(NiveauLogInfo, "Logs initialisees");
    
    #ifndef NDEBUG
    g_logger->set_immediate_flush(1u); // logs synchrones en mode debug
    #endif
}

void log_fmt(NiveauLog niveau_log, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int size = std::vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);

    std::vector<char> buf(size + 1);
    std::vsnprintf(buf.data(), buf.size(), fmt, args);
    va_end(args);

    const char* msg = buf.data();

    switch (niveau_log) {
        case NiveauLogDebug:         quill::debug(g_logger, msg); return;
        case NiveauLogInfo:          quill::info(g_logger, msg); return;
        case NiveauLogAvertissement: quill::warning(g_logger, msg); return;
        case NiveauLogErreur:        quill::error(g_logger, msg); return;
    }
}

void log_message(NiveauLog niveau_log, const char* message) {
    log_fmt(niveau_log, message);
}