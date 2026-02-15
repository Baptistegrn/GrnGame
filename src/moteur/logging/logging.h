/*
 * Systeme de logging pour le moteur.
 */

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum NiveauLog {
    NiveauLogDebug,
    NiveauLogInfo,
    NiveauLogAvertissement,
    NiveauLogErreur
} NiveauLog;

typedef enum DestinationLog { DestinationLogConsole, DestinationLogFichier } DestinationLog;

void initialiser_logging(DestinationLog destination, const char *nom_fichier, NiveauLog niveau);

void log_fmt(NiveauLog niveau, const char *fmt, ...);

void log_message(NiveauLog niveau, const char *message);

void changer_niveau_log(NiveauLog nouveau_niveau);

#ifdef __cplusplus
}
#endif

#endif