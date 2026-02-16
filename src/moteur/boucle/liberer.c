/*
 * Liberation des ressources liees au timing.
 */

#include "../../allouer/allouer.h"
#include "../../main.h"

/* Libere le GestionnaireTiming */
void liberer_timing(void) {
    if (!gs || !gs->timing)
        return;

    free_gestion_echec_compteur(gs->timing);
    gs->timing = NULL;
}
