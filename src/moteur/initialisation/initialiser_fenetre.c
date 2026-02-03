/*
 * Initialisation de la fenetre et du renderer SDL.
 */

#include "../../main.h"

/* Initialise la fenetre et le renderer */
void initialiser_fenetre(void) {
    if (!gs)
        goto gsvide;

    GestionnaireFenetre *fen = gs->fenetre;

    /* Creation fenetre */
    fen->fenetre =
        SDL_CreateWindow(fen->nom_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         fen->largeur_univers, fen->hauteur_univers, SDL_WINDOW_SHOWN);

    if (!fen->fenetre) {
        log_fmt(NiveauLogErreur, "SDL_CreateWindow failed: %s", SDL_GetError());
        gs->timing->run = false;
        return;
    }

    /* render - essaie 1: gpu */
    fen->rendu = SDL_CreateRenderer(fen->fenetre, -1, SDL_RENDERER_ACCELERATED);

    /* render - essaie 2: cpu */
    if (!fen->rendu) {
        log_message(NiveauLogAvertissement, "Hardware renderer failed, trying software mode");
        fen->rendu = SDL_CreateRenderer(fen->fenetre, -1, SDL_RENDERER_SOFTWARE);
    }

    if (!fen->rendu) {
        log_fmt(NiveauLogErreur, "SDL_CreateRenderer failed: %s", SDL_GetError());
        gs->timing->run = false;
        return;
    }

    /* configuration renderer */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // Pixel Art
    SDL_RenderSetIntegerScale(fen->rendu, SDL_TRUE);

    /* redimensionnement plein ecran pour steam deck */
    redimensionner(0, 0, true, false);

    if (gs->timing->run)
        log_message(NiveauLogDebug, "Initialisation successful: Window and renderer initialized");

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in window initialization");
}