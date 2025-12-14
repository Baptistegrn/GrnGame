#include "../../logging/logging.h"
#include "../../main.h"
#include <math.h>

void actualiser(Gestionnaire *gs, bool bande_noir) {
  if (!gs || !gs->rendu) {
    log_fmt(NiveauLogErreur,
            "Erreur: Gestionnaire ou renderer NULL dans actualiser()");
    return;
  }

  SDL_Renderer *rendu = gs->rendu;

  // Coloriage fond
  if (gs->fond->colorier_frame) {
    SDL_SetRenderDrawColor(rendu, gs->fond->r, gs->fond->g, gs->fond->b, 255);
    gs->fond->colorier_frame = false;
  } else {
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
  }

  SDL_RenderClear(rendu);
  // images
  afficher_images(gs);
  // shader
  appliquer_shaders(gs);
  // bande noir
  if (bande_noir) {
    int largeur, hauteur;
    SDL_GetWindowSize(gs->fenetre, &largeur, &hauteur);
    dessiner_bandes_noires(rendu, gs->decalage_x, gs->decalage_y, largeur,
                           hauteur);
  }

  SDL_RenderPresent(rendu);
}