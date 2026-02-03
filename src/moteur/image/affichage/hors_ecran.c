/*
 * Detection de culling pour les objets hors ecran.
 */

#include "../../../main.h"
#include "SDL_stdinc.h"

/* Verifie si un objet est situe hors de la zone visible de l'ecran */
bool hors_ecran(float posx, float posy, Sint16 taillex, Sint16 tailley, int decalage_x_scaled,
                int decalage_y_scaled) {
    if (!gs)
        goto gsvide;

    Sint16 rounded_posx = SDL_lroundf(posx);
    Sint16 rounded_posy = SDL_lroundf(posy);

    if (rounded_posx - (Sint16)decalage_x_scaled > (Sint16)gs->fenetre->largeur_univers ||
        rounded_posx + taillex + (Sint16)decalage_x_scaled < 0 ||
        rounded_posy - (Sint16)decalage_y_scaled > (Sint16)gs->fenetre->hauteur_univers ||
        rounded_posy + tailley + (Sint16)decalage_y_scaled < 0) {
        return true;
    }

    return false;

gsvide:
    log_message(NiveauLogDebug, "empty manager in culling function");
    return true;
}