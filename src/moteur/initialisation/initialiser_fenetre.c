#include "../../main.h"

/**
 * Initialise la fenêtre et le renderer, puis délègue la configuration
 * de l'affichage à appliquer_mode_affichage pour démarrer en plein écran.
 */
void initialiser_fenetre(void) {
    if (!gs)
        goto gsvide;

    GestionnaireFenetre *fen = gs->fenetre;

    /* création fenetre */
    fen->fenetre =
        SDL_CreateWindow(fen->nom_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         fen->largeur_univers, fen->hauteur_univers, SDL_WINDOW_SHOWN);

    if (!fen->fenetre) {
        log_fmt(NiveauLogErreur, "Error SDL_CreateWindow : %s", SDL_GetError());
        gs->timing->run = false;
        return;
    }

    /*render */
    fen->rendu =
        SDL_CreateRenderer(fen->fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!fen->rendu) {
        log_fmt(NiveauLogErreur, "Error SDL_CreateRenderer : %s", SDL_GetError());
        gs->timing->run = false;
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // Pixel Art (Nearest Neighbor)
    SDL_RenderSetIntegerScale(fen->rendu, SDL_TRUE);

    /* redimensionnement plein ecran pour steam deck */
    redimensionner(0, 0, true, false);

    if (gs->timing->run)
        log_message(NiveauLogDebug, "Initialisation successful: Window started in Fullscreen");

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in window initialization");
}