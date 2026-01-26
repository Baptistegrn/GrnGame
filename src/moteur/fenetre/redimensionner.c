#include "../../main.h"

void redimensionner(int w, int h, bool fullscreen_demande, bool fenetre_demande_fullscreen) {
    if (!gs)
        goto gsvide;

    GestionnaireFenetre *f = gs->fenetre;
    SDL_Window *fenetre_sdl = f->fenetre;

    if (f->largeur_univers <= 0 || f->hauteur_univers <= 0) {
        log_message(NiveauLogErreur, "Dimensions of the universe are invalid");
        return;
    }

    /* info ecran */
    int index_ecran = SDL_GetWindowDisplayIndex(fenetre_sdl);
    SDL_Rect limites_ecran;
    SDL_DisplayMode mode_bureau;

    // récupère la résolution totale (plein écran)
    if (index_ecran < 0 || SDL_GetDesktopDisplayMode(index_ecran, &mode_bureau) != 0) {
        log_message(NiveauLogErreur, "Impossible to get display information");
        return;
    }

    // récupère la zone utilisable (sans barre des tâches)
    if (SDL_GetDisplayUsableBounds(index_ecran, &limites_ecran) != 0) {
        log_message(NiveauLogErreur, "Impossible to get usable display bounds");
        return;
    }

    /* sauvegarde de la position actuelle de la souris (univers)*/
    float souris_x_jeu = 0, souris_y_jeu = 0;
    if (gs->entrees) {
        souris_x_jeu = (float)gs->entrees->souris_x;
        souris_y_jeu = (float)gs->entrees->souris_y;
    }

    int largeur_cible, hauteur_cible;
    int decalage_x = 0;
    int decalage_y = 0;

    /* si fullscreen demande */
    if (fullscreen_demande) {
        largeur_cible = mode_bureau.w;
        hauteur_cible = mode_bureau.h;

        SDL_SetWindowFullscreen(fenetre_sdl, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    /* si fenetre fullscreen demandée (barre des taches visible) */
    if (fenetre_demande_fullscreen) {
        largeur_cible = limites_ecran.w;
        hauteur_cible = limites_ecran.h;

        SDL_SetWindowFullscreen(fenetre_sdl, 0);
        SDL_SetWindowBordered(fenetre_sdl, SDL_TRUE);
        SDL_SetWindowSize(fenetre_sdl, largeur_cible, hauteur_cible);
        SDL_SetWindowPosition(fenetre_sdl, limites_ecran.x, limites_ecran.y);

        /* sinon fenetre normale */
    }
    if (!fullscreen_demande && !fenetre_demande_fullscreen) {
        largeur_cible = w;
        hauteur_cible = h;

        SDL_SetWindowFullscreen(fenetre_sdl, 0);
        SDL_SetWindowSize(fenetre_sdl, largeur_cible, hauteur_cible);

        if (largeur_cible < mode_bureau.w && hauteur_cible < mode_bureau.h) {
            SDL_SetWindowBordered(fenetre_sdl, SDL_TRUE);
        }

        /* Centrage de la fenêtre sur l'écran */
        SDL_SetWindowPosition(fenetre_sdl, limites_ecran.x + (mode_bureau.w - largeur_cible) / 2,
                              limites_ecran.y + (mode_bureau.h - hauteur_cible) / 2);
    }

    /* on cherche le plus petit coeff */
    int coeff_w = largeur_cible / f->largeur_univers;
    int coeff_h = hauteur_cible / f->hauteur_univers;
    int n_coeff = (coeff_w < coeff_h) ? coeff_w : coeff_h;
    if (n_coeff < 1)
        n_coeff = 1;

    /* largeur du jeu possible maximum */
    int largeur_jeu_reelle = f->largeur_univers * n_coeff;
    int hauteur_jeu_reelle = f->hauteur_univers * n_coeff;

    /* decalage (bandes noirs) */
    decalage_x = (largeur_cible - largeur_jeu_reelle) / 2;
    decalage_y = (hauteur_cible - hauteur_jeu_reelle) / 2;

    f->largeur_actuelle = largeur_cible;
    f->hauteur_actuelle = hauteur_cible;
    f->decalage_x = decalage_x;
    f->decalage_y = decalage_y;
    f->plein_ecran = fullscreen_demande;
    f->coeff = (unsigned char)n_coeff;

    if (gs->entrees) {
        int souris_x_physique = lroundf((souris_x_jeu * (float)n_coeff) + (float)decalage_x);
        int souris_y_physique = lroundf((souris_y_jeu * (float)n_coeff) + (float)decalage_y);
        SDL_WarpMouseInWindow(fenetre_sdl, souris_x_physique, souris_y_physique);
        gs->entrees->souris_x = souris_x_jeu;
        gs->entrees->souris_y = souris_y_jeu;
    }

    log_fmt(NiveauLogDebug, "Display updated: %s (%dx%d) | Coeff: %d | Offset: %d,%d",
            f->plein_ecran ? "Fullscreen" : "Window", largeur_cible, hauteur_cible, n_coeff,
            decalage_x, decalage_y);

    return;

gsvide:
    log_message(NiveauLogDebug, "Window manager or window is empty during resizing");
}
