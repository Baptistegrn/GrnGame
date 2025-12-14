#include "../../main.h"
#include <math.h>
#include <stdbool.h>

void dessiner_triangle_vide(Gestionnaire *gs, int posx, int posy, int largeur,
                            int hauteur, int r, int g, int b) {
  int current_w = largeur;
  int current_y = posy + hauteur;
  int y_base = posy + hauteur;

  if (largeur >= hauteur) {
    while (current_w > 0 && current_y >= posy) {
      int x_start = posx + (largeur - current_w) / 2;
      int x_end = x_start + current_w - 1;

      if (current_y == y_base) {
        tracer_ligne_horizontale(gs, x_start, current_y, current_w, r, g, b);
      } else {
        dessiner_points(gs, x_start, current_y, x_end, current_y, r, g, b);
      }

      current_y--;
      int diff = largeur - hauteur;
      if (diff > 50)
        current_w -= 4;
      else
        current_w -= 2;
    }
  } else {
    int steps = largeur / 2;
    if (steps == 0)
      steps = 1;
    int epaisseur = hauteur / steps;
    if (epaisseur < 1)
      epaisseur = 1;
    int count_epaisseur = 0;

    while (current_w > 0 && current_y >= posy) {
      int x_start = posx + (largeur - current_w) / 2;
      int x_end = x_start + current_w - 1;

      if (current_y == y_base) {
        tracer_ligne_horizontale(gs, x_start, current_y, current_w, r, g, b);
      } else {
        dessiner_points(gs, x_start, current_y, x_end, current_y, r, g, b);
      }

      current_y--;
      count_epaisseur++;

      if (count_epaisseur >= epaisseur) {
        current_w -= 2;
        count_epaisseur = 0;
      }
    }
  }
}

void dessiner_triangle_plein(Gestionnaire *gs, int posx, int posy, int largeur,
                             int hauteur, int r, int g, int b) {
  int current_w = largeur;
  int current_y = posy + hauteur;

  if (largeur >= hauteur) {
    while (current_w > 0 && current_y >= posy) {
      int x_start = posx + (largeur - current_w) / 2;
      tracer_ligne_horizontale(gs, x_start, current_y, current_w, r, g, b);

      current_y--;
      int diff = largeur - hauteur;
      if (diff > 50)
        current_w -= 4;
      else
        current_w -= 2;
    }
  } else {
    int steps = largeur / 2;
    if (steps == 0)
      steps = 1;
    int epaisseur = hauteur / steps;
    if (epaisseur < 1)
      epaisseur = 1;
    int count_epaisseur = 0;

    while (current_w > 0 && current_y >= posy) {
      int x_start = posx + (largeur - current_w) / 2;
      tracer_ligne_horizontale(gs, x_start, current_y, current_w, r, g, b);

      current_y--;
      count_epaisseur++;

      if (count_epaisseur >= epaisseur) {
        current_w -= 2;
        count_epaisseur = 0;
      }
    }
  }
}