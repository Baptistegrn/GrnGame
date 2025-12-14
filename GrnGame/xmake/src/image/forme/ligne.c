#include "../../main.h"
#include "SDL_render.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// Bresenham
void dessiner_ligne_pixel(Gestionnaire *gs, int x0, int y0, int x1, int y1,
                          int r, int g, int b) {
  int coeff = gs->largeur_actuel / gs->largeur;
  int decalage_x = gs->decalage_x;
  int decalage_y = gs->decalage_y;

  SDL_SetRenderDrawColor(gs->rendu, r, g, b, 255);

  int dx = abs(x1 - x0);
  int dy = -abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  int e2;

  while (true) {
    SDL_Rect pixel = {(x0 * coeff) + decalage_x, (y0 * coeff) + decalage_y,
                      coeff, coeff};
    SDL_RenderFillRect(gs->rendu, &pixel);

    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

// Ligne horizontale opti
void tracer_ligne_horizontale(Gestionnaire *gs, int x_start, int y, int w,
                              int r, int g, int b) {
  int coeff = gs->largeur_actuel / gs->largeur;
  SDL_SetRenderDrawColor(gs->rendu, r, g, b, 255);

  SDL_Rect ligne = {(x_start * coeff) + gs->decalage_x,
                    (y * coeff) + gs->decalage_y, w * coeff, coeff};
  SDL_RenderFillRect(gs->rendu, &ligne);
}

// points miroir avec n
void dessiner_points_n(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                       int g, int b, int n) {
  int coeff = gs->largeur_actuel / gs->largeur;
  SDL_SetRenderDrawColor(gs->rendu, r, g, b, 255);

  SDL_Rect p1 = {(x1 * coeff) + gs->decalage_x, (y1 * coeff) + gs->decalage_y,
                 coeff * n, coeff};
  SDL_RenderFillRect(gs->rendu, &p1);

  if (x1 != x2 || y1 != y2) {
    SDL_Rect p2 = {((x2 - n) * coeff) + gs->decalage_x,
                   (y2 * coeff) + gs->decalage_y, coeff * n, coeff};
    SDL_RenderFillRect(gs->rendu, &p2);
  }
}

void dessiner_points(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                     int g, int b) {
  int coeff = gs->largeur_actuel / gs->largeur;
  SDL_SetRenderDrawColor(gs->rendu, r, g, b, 255);

  SDL_Rect p1 = {(x1 * coeff) + gs->decalage_x, (y1 * coeff) + gs->decalage_y,
                 coeff, coeff};
  SDL_RenderFillRect(gs->rendu, &p1);

  if (x1 != x2 || y1 != y2) {
    SDL_Rect p2 = {((x2)*coeff) + gs->decalage_x, (y2 * coeff) + gs->decalage_y,
                   coeff, coeff};
    SDL_RenderFillRect(gs->rendu, &p2);
  }
}