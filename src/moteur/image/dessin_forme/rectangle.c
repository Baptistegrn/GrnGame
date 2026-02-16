/*
 * Dessin de rectangles (vides et pleins).
 */

#include "../../../main.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "dessin.h"
#include <stdbool.h>


/* Dessine un rectangle vide en tracant 4 lignes */
void dessiner_rectangle_vide(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                             Uint8 a) {

    float x2 = x + (float)w - 1.0f;
    float y2 = y + (float)h - 1.0f;

    float w_sans_point_haut = (w > 1) ? (float)(w - 1) : 0.0f;
    float h_sans_point_haut = (h > 2) ? (float)(h - 2) : 0.0f;

    tracer_ligne_verticale_float(x, y, (float)h, r, g, b, a);
    if (w > 1) {
        tracer_ligne_horizontale_float(x + 1.0f, y, w_sans_point_haut, r, g, b, a);
    }
    if (h > 1 && w > 1) {
        tracer_ligne_horizontale_float(x + 1.0f, y2, w_sans_point_haut, r, g, b, a);
    }
    if (w > 1 && h > 1) {
        tracer_ligne_verticale_float(x2, y + 1.0f, h_sans_point_haut, r, g, b, a);
    }
}
/* Dessine un rectangle rempli avec la couleur specifiee */
void dessiner_rectangle_plein(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                              Uint8 a) {
    if (!gs)
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);

    SDL_Rect rect = {lroundf((x * (float)coeff) + (float)gs->fenetre->decalage_x),
                     lroundf((y * (float)coeff) + (float)gs->fenetre->decalage_y), (int)(w * coeff),
                     (int)(h * coeff)};
    SDL_RenderFillRect(gs->fenetre->rendu, &rect);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw filled rectangle");
}
