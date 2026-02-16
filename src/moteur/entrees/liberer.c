/*
 * Liberation des ressources liees aux entrees (clavier, manette).
 */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "entrees.h"

/* Libere le GestionnaireEntrees */
void liberer_entrees(void) {
    if (!gs || !gs->entrees)
        return;

    free_gestion_echec_compteur(gs->entrees);
    gs->entrees = NULL;
}

/* Libere les controleurs (manette et joystick) */
void liberer_manette(void) {
    if (!gs)
        return;
    for (int i = 0; i < NB_MANETTES_MAX; i++) {
        if (gs->entrees->man[i].manette != NULL) {
            fermer_controller(i);
            gs->entrees->man[i].manette = NULL;
        }

        if (gs->entrees->man[i].joystick) {
            fermer_joystick(i);
            gs->entrees->man[i].joystick = NULL;
        }
    }
}
