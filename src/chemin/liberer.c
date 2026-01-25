#include "../main.h"
#include "chemin.h"

/*
 * Libère la structure Fichiers et son tableau de noms.
 */
void liberer_fichiers(Fichiers *fichiers) {
    if (fichiers == NULL) {
        return;
    }
    if (fichiers->noms != NULL) {
        free(fichiers->noms);
    }
    free(fichiers);
}