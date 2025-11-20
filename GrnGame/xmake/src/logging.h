#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum NiveauLog {
    NiveauLogDebug,
    NiveauLogInfo,
    NiveauLogAvertissement,
    NiveauLogErreur,
};

enum DestinationLog {
    DestinationLogConsole,
    DestinationLogFichier,
};

void initialiser_logging(enum DestinationLog destination_log, const char* nom_fichier, enum NiveauLog niveau_log);

void log_fmt(enum NiveauLog niveau_log, const char* fmt, ...);
void log_message(enum NiveauLog, const char* message);
void changer_niveau_log(enum NiveauLog nouveau_niveau_log);
#ifdef __cplusplus
}
#endif