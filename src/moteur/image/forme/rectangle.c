/*
 * Dessin de rectangles (vides et pleins).
 */

#include "../../../main.h"

/* Dessine un rectangle vide en tracant 4 lignes */
void dessiner_rectangle_vide(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                             Uint8 a) {
    float wr = (float)w;
    float hr = (float)h;
    dessiner_ligne_pixel(x, y, x + wr, y, r, g, b, a);
    dessiner_ligne_pixel(x, y + hr, x + wr, y + hr, r, g, b, a);
    dessiner_ligne_pixel(x, y, x, y + hr, r, g, b, a);
    dessiner_ligne_pixel(x + wr, y, x + wr, y + hr, r, g, b, a);
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
