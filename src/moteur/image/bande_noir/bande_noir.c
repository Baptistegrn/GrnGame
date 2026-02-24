/*
 * Dessin des bandes noires de letterboxing.
 */

#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include <stdint.h>


/* Dessine les bandes noires de padding sur les bordures de l'ecran */
void dessiner_bandes_noires(int decalage_x, int decalage_y) {
    if (UNLIKELY(!gs))
        goto gsvide;
    int largeur = 0;
    int hauteur = 0;

    SDL_GetWindowSize(gs->fenetre->fenetre, &largeur, &hauteur);
    SDL_Renderer *rendu = gs->fenetre->rendu;
    SDL_SetRenderDrawColor(rendu, 10, 10, 10, 255);

    SDL_Rect rect_gauche = {0, 0, decalage_x, hauteur};
    SDL_RenderFillRect(rendu, &rect_gauche);

    SDL_Rect rect_droite = {largeur - decalage_x, 0, decalage_x, hauteur};
    SDL_RenderFillRect(rendu, &rect_droite);

    SDL_Rect rect_haut = {0, 0, largeur, decalage_y};
    SDL_RenderFillRect(rendu, &rect_haut);

    SDL_Rect rect_bas = {0, hauteur - decalage_y, largeur, decalage_y};
    SDL_RenderFillRect(rendu, &rect_bas);
    return;

gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in draw black bands function");
}