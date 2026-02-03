/*
 * Liberation des ressources liees au son.
 */

#include "../../main.h"
#include <stdlib.h>

/* Libere les ressources audio (Chunks) et le tableau du gestionnaire de son */
void liberer_gestionnaire_son(void) {
    if (!gs)
        goto gsvide;

    GestionnaireSon *sons = gs->sons;

    for (int i = 0; i < sons->taille; i++) {
        if (sons->entrees[i].son) {
            Mix_FreeChunk(sons->entrees[i].son);
            sons->entrees[i].son = NULL;
        }
    }

    if (sons->entrees) {
        free_gestion_echec_compteur(sons->entrees);
        sons->entrees = NULL;
    }

    sons->taille = 0;
    sons->capacite = 0;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in free sound manager function");
}

/* Libere le GestionnaireSon */
void liberer_sons(void) {
    if (!gs || !gs->sons)
        return;

    liberer_gestionnaire_son();
    free_gestion_echec_compteur(gs->sons);
    gs->sons = NULL;
}
