/*
 * Gestion de la couleur de fond.
 */

#include "../../main.h"
#include "../boucle/boucle.h"
#include "../logging/logging.h"
#include "fenetre.h"

/* Stocke les informations de coloriage pour la frame courante */
void stocker_coloriage(int r, int g, int b) {
    if (!gs)
        goto gsvide;
    fondActualiser *fond = gs->frame->fond;

    /* Limitation des valeurs RGB entre 0 et 255 */
    r = SDL_clamp(r, 0, 255);
    g = SDL_clamp(g, 0, 255);
    b = SDL_clamp(b, 0, 255);

    fond->r = r;
    fond->g = g;
    fond->b = b;
    fond->colorier_frame = true;

    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in draw color function");
}
