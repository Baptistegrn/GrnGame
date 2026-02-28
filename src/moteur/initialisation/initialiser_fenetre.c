/*
 * Initialisation de la fenetre et du renderer SDL.
 */

#include "../../main.h"
#include "../../prediction_branche.h"
#include "../boucle/boucle.h"
#include "../fenetre/fenetre.h"
#include "../logging/logging.h"

/* Initialise la fenetre et le renderer */
void initialiser_fenetre(void) {
    if (UNLIKELY(!gs))
        goto gsvide;

    GestionnaireFenetre *fen = gs->fenetre;
    Uint32 flags_fenetre = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

    /* Creation fenetre */
    fen->fenetre =
        SDL_CreateWindow(fen->nom_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         fen->largeur_univers, fen->hauteur_univers, flags_fenetre);

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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); /* Pixel Art */
    SDL_RenderSetScale(fen->rendu, 1.0f, 1.0f);
    SDL_RenderSetIntegerScale(fen->rendu, SDL_FALSE);
    /* redimensionnement plein ecran pour steam deck */
    passer_plein_ecran();

    if (gs->timing->run)
        log_message(NiveauLogDebug, "Initialisation successful: Window and renderer initialized");

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in window initialization");
}