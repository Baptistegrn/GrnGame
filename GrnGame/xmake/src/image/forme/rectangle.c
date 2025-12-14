#include "../../main.h"
#include "SDL_render.h"
#include <math.h>
#include <stdbool.h>

void dessiner_rectangle_vide(Gestionnaire *gs, int x, int y, int w, int h,
                             int r, int g, int b) {
  dessiner_ligne_pixel(gs, x, y, x + w, y, r, g, b);
  dessiner_ligne_pixel(gs, x, y + h, x + w, y + h, r, g, b);
  dessiner_ligne_pixel(gs, x, y, x, y + h, r, g, b);
  dessiner_ligne_pixel(gs, x + w, y, x + w, y + h, r, g, b);
}

void dessiner_rectangle_plein(Gestionnaire *gs, int x, int y, int w, int h,
                              int r, int g, int b) {
  int coeff = gs->largeur_actuel / gs->largeur;

  SDL_SetRenderDrawColor(gs->rendu, r, g, b, 255);
  SDL_Rect rect = {(x * coeff) + gs->decalage_x, (y * coeff) + gs->decalage_y,
                   w * coeff, h * coeff};
  SDL_RenderFillRect(gs->rendu, &rect);
}
