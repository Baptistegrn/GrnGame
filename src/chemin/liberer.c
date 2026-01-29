#include "../main.h"

/*
 * Libère la structure Fichiers et son tableau de noms.
 */
void liberer_fichiers(Fichiers *fichiers) {
    if (fichiers == NULL) {
        return;
    }
    if (fichiers->noms != NULL) {
        xfree(fichiers->noms);
    }
    if (fichiers->temps != NULL) {
        xfree(fichiers->temps);
    }
    xfree(fichiers);
}