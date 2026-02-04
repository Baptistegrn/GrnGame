/*
 * Dessin de cercles (vides et pleins).
 */

#include "../../../main.h"

/* Dessine un cercle vide avec l'algorithme de Bresenham */
void dessiner_cercle_vide(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (rayon <= 0)
        return; /* protection */

    for (Sint16 dy = 0; dy < rayon; dy++) {
        Sint16 demi_largeur = (Sint16)sqrt((double)(rayon * rayon - dy * dy));
        Sint16 demi_largeur_suiv = (Sint16)sqrt((double)(rayon * rayon - (dy + 1) * (dy + 1)));

        Sint16 n = demi_largeur - demi_largeur_suiv;
        if (n < 1)
            n = 1;

        float x1 = xc - (float)demi_largeur;
        float x2 = xc + (float)demi_largeur;

        if (dy == 0) {
            dessiner_points_n(x1, yc, x2, yc, r, g, b, a, n);
        } else if (dy == rayon - 1) {
            dessiner_ligne_pixel(x1, yc - (float)dy, x2 - 1.0f, yc - (float)dy, r, g, b, a);
            dessiner_ligne_pixel(x1, yc + (float)dy, x2 - 1.0f, yc + (float)dy, r, g, b, a);
        } else {
            dessiner_points_n(x1, yc - (float)dy, x2, yc - (float)dy, r, g, b, a, n);
            dessiner_points_n(x1, yc + (float)dy, x2, yc + (float)dy, r, g, b, a, n);
        }
    }
}

/* Tracage ligne par ligne selon la longueur de pythagore */
void dessiner_cercle_plein(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    for (Sint16 dy = 0; dy <= rayon; dy++) {
        Sint16 demi_largeur = (Sint16)sqrt(rayon * rayon - dy * dy);
        Sint16 largeur_ligne = demi_largeur * 2;
        float x_start = xc - (float)demi_largeur;

        if (dy == 0) {
            tracer_ligne_horizontale(x_start, yc, largeur_ligne, r, g, b, a);
        } else {
            tracer_ligne_horizontale(x_start, yc - (float)dy, largeur_ligne, r, g, b, a);
            tracer_ligne_horizontale(x_start, yc + (float)dy, largeur_ligne, r, g, b, a);
        }
    }
}
