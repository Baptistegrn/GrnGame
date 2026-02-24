/*
 * Configuration de la redirection des erreurs vers un fichier log.
 */

#include "../../allouer/allouer.h"
#include "../../prediction_branche.h"
#include "../logging/logging.h"


/* Configure la redirection des erreurs vers un fichier log */
void configurer_redirection_stderr(const char *chemin_log) {
    if (UNLIKELY(!chemin_log)) {
        initialiser_logging(DestinationLogConsole, NULL, NiveauLogInfo);
        return;
    }

    initialiser_logging(DestinationLogFichier, chemin_log, NiveauLogInfo);

    log_message(NiveauLogInfo, "logs correctly redirected to file");
}
