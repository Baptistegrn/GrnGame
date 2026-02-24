/*
 * Actualisation du rendu graphique.
 */

#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../boucle/boucle.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "../bande_noir/bande_noir.h"
#include "affichage.h"

/* Actualise le rendu graphique complet de la frame */
void actualiser() {
    if (UNLIKELY(!gs))
        goto gsvide;

    SDL_Renderer *rendu = gs->fenetre->rendu;
    bool bande_noir = gs->frame->fond->bande_noir;

    if (gs->frame->fond->colorier_frame) {
        SDL_SetRenderDrawColor(rendu, gs->frame->fond->r, gs->frame->fond->g, gs->frame->fond->b,
                               255);
        gs->frame->fond->colorier_frame = false;
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