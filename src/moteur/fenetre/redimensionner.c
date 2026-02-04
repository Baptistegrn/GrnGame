/*
 * Redimensionnement de la fenetre et gestion du letterboxing.
 * Calcule le coefficient de mise a l'echelle et les decalages.
 */

#include "../../main.h"

/* Fonction pour calculer le coefficient et appliquer le redimensionnement (appel dans le event sdl)
 */
void appliquer_redimensionnement(int largeur_cible, int hauteur_cible) {
    GestionnaireFenetre *f = gs->fenetre;

    /* plus petit coefficient */
    int coeff_w = largeur_cible / f->largeur_univers;
    int coeff_h = hauteur_cible / f->hauteur_univers;
    int n_coeff = (coeff_w < coeff_h) ? coeff_w : coeff_h;
    if (n_coeff < 1)
        n_coeff = 1;

    /* Largeur du jeu possible maximum */
    int largeur_jeu_reelle = f->largeur_univers * n_coeff;
    int hauteur_jeu_reelle = f->hauteur_univers * n_coeff;

    /* Decalage (bandes noires) */
    int decalage_x = (largeur_cible - largeur_jeu_reelle) / 2;
    int decalage_y = (hauteur_cible - hauteur_jeu_reelle) / 2;

    f->largeur_actuelle = largeur_cible;
    f->hauteur_actuelle = hauteur_cible;
    f->decalage_x = decalage_x;
    f->decalage_y = decalage_y;
    f->coeff = (unsigned char)n_coeff;

    log_fmt(NiveauLogDebug, "Display updated: %s (%dx%d) | Coeff: %d | Offset: %d,%d",
            f->plein_ecran ? "Fullscreen" : "Window", largeur_cible, hauteur_cible, n_coeff,
            decalage_x, decalage_y);
}

/* Passe en mode plein ecran */
void passer_plein_ecran(void) {
    if (!gs)
        goto gsvide;

    GestionnaireFenetre *f = gs->fenetre;
    SDL_Window *fenetre_sdl = f->fenetre;

    int index_ecran = SDL_GetWindowDisplayIndex(fenetre_sdl);
    SDL_DisplayMode mode_bureau;

    if (index_ecran < 0 || SDL_GetDesktopDisplayMode(index_ecran, &mode_bureau) != 0) {
        log_message(NiveauLogErreur, "Impossible to get display information");
        return;
    }

    SDL_SetWindowFullscreen(fenetre_sdl, SDL_WINDOW_FULLSCREEN_DESKTOP);
    f->plein_ecran = true;
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in passer_plein_ecran");
}

/* Passe en mode fenetre maximisee (avec barre des taches et titre) */
void passer_fenetre_maximisee(void) {
    if (!gs || !gs->fenetre) {
        log_message(NiveauLogDebug, "manager is empty in passer_fenetre_maximisee");
        return;
    }

    SDL_Window *fenetre_sdl = gs->fenetre->fenetre;

    /* quitter le plein ecran */
    SDL_SetWindowFullscreen(fenetre_sdl, 0);

    /* autoriser le redimensionnement */
    SDL_SetWindowResizable(fenetre_sdl, SDL_TRUE);

    /* remettre les bordures et le titre */
    SDL_SetWindowBordered(fenetre_sdl, SDL_TRUE);

    /* maximiser la fenetre */
    SDL_MaximizeWindow(fenetre_sdl);
    gs->fenetre->plein_ecran = false;
}

/* Passe en mode fenetre avec une taille specifique */
void passer_fenetre_taille(int largeur, int hauteur) {
    if (!gs)
        goto gsvide;

    GestionnaireFenetre *f = gs->fenetre;
    SDL_Window *fenetre_sdl = f->fenetre;

    int index_ecran = SDL_GetWindowDisplayIndex(fenetre_sdl);
    SDL_DisplayMode mode_bureau;
    SDL_Rect limites_ecran;

    if (index_ecran < 0 || SDL_GetDesktopDisplayMode(index_ecran, &mode_bureau) != 0) {
        log_message(NiveauLogErreur, "Impossible to get display information");
        return;
    }

    if (SDL_GetDisplayUsableBounds(index_ecran, &limites_ecran) != 0) {
        log_message(NiveauLogErreur, "Impossible to get usable display bounds");
        return;
    }

    SDL_SetWindowFullscreen(fenetre_sdl, 0);
    SDL_SetWindowBordered(fenetre_sdl, SDL_TRUE);
    SDL_SetWindowSize(fenetre_sdl, largeur, hauteur);

    /* Centrage de la fenetre sur l'ecran */
    SDL_SetWindowPosition(fenetre_sdl, limites_ecran.x + (limites_ecran.w - largeur) / 2,
                          limites_ecran.y + (limites_ecran.h - hauteur) / 2);
    f->plein_ecran = false;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in passer_fenetre_taille");
}

/* Passe en mode fenetre avec un coefficient de l'univers */
void passer_fenetre_coeff(int coeff) {
    if (!gs)
        goto gsvide;

    if (coeff < 1)
        coeff = 1;

    GestionnaireFenetre *f = gs->fenetre;
    int largeur = f->largeur_univers * coeff;
    int hauteur = f->hauteur_univers * coeff;

    passer_fenetre_taille(largeur, hauteur);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in passer_fenetre_coeff");
}
