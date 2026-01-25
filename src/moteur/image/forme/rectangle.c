#include "../../../main.h"

/*
 * Dessine un rectangle vide en traçant 4 lignes (haut, bas, gauche, droite).
 */
void dessiner_rectangle_vide(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b) {
    float wr = (float)w;
    float hr = (float)h;
    dessiner_ligne_pixel(x, y, x + wr, y, r, g, b);
    dessiner_ligne_pixel(x, y + hr, x + wr, y + hr, r, g, b);
    dessiner_ligne_pixel(x, y, x, y + hr, r, g, b);
    dessiner_ligne_pixel(x + wr, y, x + wr, y + hr, r, g, b);
}

/*
 * Dessine un rectangle rempli avec la couleur spécifiée.
 * Applique l'échelle et le décalage écran.
 */
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
