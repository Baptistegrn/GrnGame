/*
 * Dessin de cercles (vides et pleins).
 */

#include "dessin.h"
#include <math.h>


/* Dessine un cercle vide avec épaisseur et lignes horizontales en haut/bas */
void dessiner_cercle_vide(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    char decalage = 0;
    for (Sint16 dy = 0; dy <= rayon; dy++) {
        Sint16 demi_largeur = (Sint16)sqrt((double)(rayon * rayon - dy * dy));
        Sint16 demi_largeur_suiv =
            (dy + 1 <= rayon) ? (Sint16)sqrt((double)(rayon * rayon - (dy + 1) * (dy + 1))) : 0;
        float epaisseur = (float)(demi_largeur - demi_largeur_suiv);
        if (epaisseur < 1.0f)
            epaisseur = 1.0f;

        float x_gauche = xc - (float)demi_largeur;
        float x_droite = xc + (float)demi_largeur;
        float x_start = xc - (float)demi_largeur;
        Sint16 largeur_ligne = demi_largeur * 2;

        if (dy == 0) {
            /* si rayon plus grand que 6 alors on dessine pas un point plus long au centre*/
            if (rayon >= 6) {
                dessiner_points(x_gauche + 1.0f, yc, x_droite - 2.0f, yc, r, g, b, a);
            } else {
                dessiner_points(x_gauche, yc, x_droite - 1.0f, yc, r, g, b, a);
            }
        } else if (dy == rayon) {
            /* Bas : lignes horizontales */
            tracer_ligne_horizontale_float(x_start, yc - (float)dy, (float)largeur_ligne, r, g, b,
                                           a);
            tracer_ligne_horizontale_float(x_start, yc + (float)dy, (float)largeur_ligne, r, g, b,
                                           a);
        } else {
            /* Milieu : points epais aux cotes */
            dessiner_points_n(x_gauche, yc - (float)dy, x_droite, yc - (float)dy, r, g, b, a,
                              epaisseur);
            dessiner_points_n(x_gauche, yc + (float)dy, x_droite, yc + (float)dy, r, g, b, a,
                              epaisseur);
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
            if (rayon >= 6) {
                /* si rayon plus grand que 6 alors on dessine pas un point plus long au centre*/
                tracer_ligne_horizontale_float(x_start + 1.0f, yc, (float)largeur_ligne - 2.0f, r,
                                               g, b, a);
            } else {
                tracer_ligne_horizontale_float(x_start, yc, (float)largeur_ligne, r, g, b, a);
            }

        } else {
            tracer_ligne_horizontale_float(x_start, yc - (float)dy, (float)largeur_ligne, r, g, b,
                                           a);
            tracer_ligne_horizontale_float(x_start, yc + (float)dy, (float)largeur_ligne, r, g, b,
                                           a);
        }
    }
}
