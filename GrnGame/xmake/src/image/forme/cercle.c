#include "../../main.h"
#include <math.h>
#include <stdbool.h>

void dessiner_cercle_vide(Gestionnaire *gs, int xc, int yc, int rayon, int r,
                          int g, int b) {
  int n = 1;

  for (int dy = 0; dy < rayon; dy++) {
    int demi_largeur = (int)sqrt(rayon * rayon - dy * dy);
    int demi_largeur_suiv = (int)sqrt(rayon * rayon - (dy + 1) * (dy + 1));

    n = demi_largeur - demi_largeur_suiv;
    if (n < 1)
      n = 1;

    int x1 = xc - demi_largeur;
    int x2 = xc + demi_largeur;

    if (dy == 0) {
      dessiner_points_n(gs, x1, yc, x2, yc, r, g, b, n);
    } else if (dy == rayon - 1) {
      dessiner_ligne_pixel(gs, x1, yc - dy, x2 - 1, yc - dy, r, g, b);
      dessiner_ligne_pixel(gs, x1, yc + dy, x2 - 1, yc + dy, r, g, b);
    } else {
      dessiner_points_n(gs, x1, yc - dy, x2, yc - dy, r, g, b, n);
      dessiner_points_n(gs, x1, yc + dy, x2, yc + dy, r, g, b, n);
    }
  }
}

void dessiner_cercle_plein(Gestionnaire *gs, int xc, int yc, int rayon, int r,
                           int g, int b) {
  for (int dy = 0; dy <= rayon; dy++) {
    int demi_largeur = (int)sqrt(rayon * rayon - dy * dy);
    int largeur_ligne = demi_largeur * 2;
    int x_start = xc - demi_largeur;

    if (dy == 0) {
      tracer_ligne_horizontale(gs, x_start, yc, largeur_ligne, r, g, b);
    } else {
      tracer_ligne_horizontale(gs, x_start, yc - dy, largeur_ligne, r, g, b);
      tracer_ligne_horizontale(gs, x_start, yc + dy, largeur_ligne, r, g, b);
    }
  }
}
