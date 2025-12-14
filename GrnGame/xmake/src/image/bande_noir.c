#include "../main.h"
#include <math.h>

void dessiner_bandes_noires(SDL_Renderer *rendu, int decalage_x, int decalage_y,
                            int largeur, int hauteur) {
  if (!rendu)
    return;

  SDL_SetRenderDrawColor(rendu, 10, 10, 10, 255);

  SDL_Rect rect_gauche = {0, 0, decalage_x, hauteur};
  SDL_RenderFillRect(rendu, &rect_gauche);

  SDL_Rect rect_droite = {largeur - decalage_x, 0, decalage_x, hauteur};
  SDL_RenderFillRect(rendu, &rect_droite);

  SDL_Rect rect_haut = {0, 0, largeur, decalage_y};
  SDL_RenderFillRect(rendu, &rect_haut);

  SDL_Rect rect_bas = {0, hauteur - decalage_y, largeur, decalage_y};
  SDL_RenderFillRect(rendu, &rect_bas);
}