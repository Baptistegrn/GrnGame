/*
 * Allocation des structures principales du gestionnaire.
 * Initialise toutes les sous-structures avec une memoire a zero.
 */

#include "../../main.h"

/* Alloue les structures principales du gestionnaire */
Gestionnaire *allouer_structures(void) {
    Gestionnaire *gs = malloc_gestion_echec_compteur(sizeof(Gestionnaire));
    memset(gs, 0, sizeof(Gestionnaire));

    gs->frame = malloc_gestion_echec_compteur(sizeof(GestionnaireFrameActuelle));
    memset(gs->frame, 0, sizeof(GestionnaireFrameActuelle));

    gs->frame->fond = malloc_gestion_echec_compteur(sizeof(fondActualiser));
    memset(gs->frame->fond, 0, sizeof(fondActualiser));

    gs->entrees = malloc_gestion_echec_compteur(sizeof(GestionnaireEntrees));
    memset(gs->entrees, 0, sizeof(GestionnaireEntrees));

    gs->frame->image = malloc_gestion_echec_compteur(sizeof(TableauImage));
    memset(gs->frame->image, 0, sizeof(TableauImage));

    gs->textures = malloc_gestion_echec_compteur(sizeof(GestionnaireTextures));
    memset(gs->textures, 0, sizeof(GestionnaireTextures));

    gs->sons = malloc_gestion_echec_compteur(sizeof(GestionnaireSon));
    memset(gs->sons, 0, sizeof(GestionnaireSon));

    gs->fenetre = malloc_gestion_echec_compteur(sizeof(GestionnaireFenetre));
    memset(gs->fenetre, 0, sizeof(GestionnaireFenetre));

    gs->timing = malloc_gestion_echec_compteur(sizeof(GestionnaireTiming));
    memset(gs->timing, 0, sizeof(GestionnaireTiming));

    log_message(NiveauLogDebug, "structures allocated successfully");
    return gs;
}
