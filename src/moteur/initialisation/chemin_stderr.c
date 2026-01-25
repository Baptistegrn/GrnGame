#include "../../main.h"

/*
 * Récupère le dernier point et garde le préfixe (sans l'extension).
 * pas utilise ici
 */
char *couper_extension(const char *texte) {
    const char *pos = strrchr(texte, '.');
    int len;
    if (!pos) {
        len = strlen(texte);
    } else {
        len = pos - texte;
    }

    char *resultat = malloc(len + 1);
    if (!resultat)
        return NULL;

    strncpy(resultat, texte, len);
    resultat[len] = '\0';
    return resultat;
}

/*
 * Configure la redirection des erreurs vers un fichier log spécifique.
 */
void configurer_redirection_stderr(const char *chemin_log) {
    if (!chemin_log) {
        initialiser_logging(DestinationLogConsole, NULL, NiveauLogInfo);
        return;
    }

    initialiser_logging(DestinationLogFichier, chemin_log, NiveauLogInfo);

    log_message(NiveauLogInfo, "logs correctly redirected to file");
}
