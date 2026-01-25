#include "../../main.h"

/*
 * Alloue les structures principales du gestionnaire.
 * Initialise l'ensemble des sous-structures : fond, shader, entrées,
 * images, textures, sons, fenêtre et timing avec allocation mémoire nulle.
 */
Gestionnaire *allouer_structures(void) {
    Gestionnaire *gs = xmalloc(sizeof(Gestionnaire));
    memset(gs, 0, sizeof(Gestionnaire));

    gs->fond = xmalloc(sizeof(fondActualiser));
    memset(gs->fond, 0, sizeof(fondActualiser));

    gs->entrees = xmalloc(sizeof(GestionnaireEntrees));
    memset(gs->entrees, 0, sizeof(GestionnaireEntrees));

    gs->image = xmalloc(sizeof(TableauImage));
    memset(gs->image, 0, sizeof(TableauImage));

    gs->textures = xmalloc(sizeof(GestionnaireTextures));
    memset(gs->textures, 0, sizeof(GestionnaireTextures));

    gs->sons = xmalloc(sizeof(GestionnaireSon));
    memset(gs->sons, 0, sizeof(GestionnaireSon));

    gs->fenetre = xmalloc(sizeof(GestionnaireFenetre));
    memset(gs->fenetre, 0, sizeof(GestionnaireFenetre));

    gs->timing = xmalloc(sizeof(GestionnaireTiming));
    memset(gs->timing, 0, sizeof(GestionnaireTiming));

    log_message(NiveauLogDebug, "structures allocated successfully");
    return gs;
}
