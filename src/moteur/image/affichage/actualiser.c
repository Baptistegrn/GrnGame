#include "../../../main.h"

/*
 * Actualise le rendu graphique complet de la frame.
 * Applique la couleur de fond, affiche les images et shaders,
 * dessine les bandes noires de letterboxing si activées,
 * puis présente le rendu final à l'écran.
 */
void actualiser() {
    if (!gs)
        goto gsvide;
    SDL_Renderer *rendu = gs->fenetre->rendu;
    bool bande_noir = gs->fond->bande_noir;

    if (gs->fond->colorier_frame) {
        SDL_SetRenderDrawColor(rendu, gs->fond->r, gs->fond->g, gs->fond->b, 255);
        gs->fond->colorier_frame = false;
    } else {
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    }
    SDL_RenderClear(rendu);

    afficher_images();

    if (bande_noir && gs->fenetre->fenetre) {

        dessiner_bandes_noires(gs->fenetre->decalage_x, gs->fenetre->decalage_y);
    }

    SDL_RenderPresent(rendu);

    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in draw update function");
}