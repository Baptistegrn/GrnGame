/*
 * Liberation de la structure Fichiers.
 */

#include "../main.h"

/* Libere une structure Fichiers et ses tableaux */
void liberer_fichiers(Fichiers *fichiers) {
    if (fichiers == NULL) {
        return;
    }
    if (fichiers->noms != NULL) {
        free_gestion_echec_compteur(fichiers->noms);
    }
    if (fichiers->temps != NULL) {
        free_gestion_echec_compteur(fichiers->temps);
    }
    free_gestion_echec_compteur(fichiers);
}