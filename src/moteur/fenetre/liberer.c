/*
 * Liberation des ressources liees a la fenetre.
 */

#include "../../main.h"
#include <stdlib.h>

/* Libere le GestionnaireFenetre et ses ressources SDL */
void liberer_fenetre(void) {
    if (!gs || !gs->fenetre)
        return;

    GestionnaireFenetre *f = gs->fenetre;

    if (f->rendu) {
        SDL_DestroyRenderer(f->rendu);
        f->rendu = NULL;
    }

    if (f->fenetre) {
        SDL_DestroyWindow(f->fenetre);
        f->fenetre = NULL;
    }

    free_gestion_echec_compteur(gs->fenetre);
    gs->fenetre = NULL;
}

/* Libere le fond_actualiser */
void liberer_fond(void) {
    if (!gs || !gs->frame->fond)
        return;

    free_gestion_echec_compteur(gs->frame->fond);
    gs->frame->fond = NULL;
}
