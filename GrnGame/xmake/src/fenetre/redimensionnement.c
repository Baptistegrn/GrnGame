#include "../main.h"
#include "../logging/logging.h" 

#include <SDL.h>
#include <SDL_image.h>

void redimensionner_fenetre(Gestionnaire* gestionnaire)
{
    if (!gestionnaire)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire NULL dans redimensionner_fenetre()");
        return;
    }

    if (!gestionnaire->fenetre || !gestionnaire->rendu)
    {
        log_fmt(NiveauLogErreur, "Erreur: Fenêtre ou renderer NULL");
        return;
    }

    SDL_Window* fenetre = gestionnaire->fenetre;
    int largeur_base = gestionnaire->largeur;
    int hauteur_base = gestionnaire->hauteur;
    int coeff_minimise = gestionnaire->coeff_minimise;
    bool plein_ecran = gestionnaire->plein_ecran;

    // Récupération des informations de l'écran
    int display_index = SDL_GetWindowDisplayIndex(fenetre);
    if (display_index < 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'obtenir l'index de l'écran: %s", SDL_GetError());
        return;
    }

    SDL_Rect display_bounds;
    if (SDL_GetDisplayBounds(display_index, &display_bounds) != 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'obtenir les dimensions de l'écran: %s", SDL_GetError());
        return;
    }

    SDL_DisplayMode mode;
    if (SDL_GetCurrentDisplayMode(display_index, &mode) != 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'obtenir le mode d'affichage: %s", SDL_GetError());
        return;
    }

    // Sauvegarde de la position de la souris dans l'univers du jeu
    float mouse_x_univers = 0.0f;
    float mouse_y_univers = 0.0f;
    if (gestionnaire->entrees)
    {
        mouse_x_univers = gestionnaire->entrees->mouse_x;
        mouse_y_univers = gestionnaire->entrees->mouse_y;
    }

    int nouvelle_largeur, nouvelle_hauteur;
    int nouveau_decalage_x, nouveau_decalage_y;

    if (plein_ecran)
    {
        
        nouveau_decalage_x = 0;
        nouveau_decalage_y = 0;
        nouvelle_largeur = largeur_base * coeff_minimise;
        nouvelle_hauteur = hauteur_base * coeff_minimise;

        SDL_SetWindowFullscreen(fenetre, 0);
        SDL_SetWindowSize(fenetre, nouvelle_largeur, nouvelle_hauteur);
        
        SDL_SetWindowPosition(fenetre, 
            display_bounds.x + (mode.w - nouvelle_largeur) / 2,
            display_bounds.y + (mode.h - nouvelle_hauteur) / 2
        );
        SDL_SetWindowBordered(fenetre, SDL_TRUE);
    }
    else
    {

        int coeff_w = mode.w / largeur_base;
        int coeff_h = mode.h / hauteur_base;

        int coeff = (coeff_w < coeff_h) ? coeff_w : coeff_h;

        if (coeff < 1) coeff = 1;

        nouvelle_largeur = largeur_base * coeff;
        nouvelle_hauteur = hauteur_base * coeff;

        nouveau_decalage_x = (mode.w - nouvelle_largeur) / 2;
        nouveau_decalage_y = (mode.h - nouvelle_hauteur) / 2;

        SDL_SetWindowFullscreen(fenetre, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }


        gestionnaire->largeur_actuel = nouvelle_largeur;
        gestionnaire->hauteur_actuel = nouvelle_hauteur;

    
    gestionnaire->decalage_x = nouveau_decalage_x;
    gestionnaire->decalage_y = nouveau_decalage_y;
    gestionnaire->plein_ecran = !plein_ecran;

    // Repositionnement de la souris pour maintenir sa position relative
    if (gestionnaire->entrees)
    {
        int coeff_actuel = nouvelle_largeur / largeur_base;

        int mouse_x_screen = (mouse_x_univers * coeff_actuel) + nouveau_decalage_x;
        int mouse_y_screen = (mouse_y_univers * coeff_actuel) + nouveau_decalage_y;

        SDL_WarpMouseInWindow(fenetre, mouse_x_screen, mouse_y_screen);

        // Conservation de la position dans l'univers du jeu
        gestionnaire->entrees->mouse_x = mouse_x_univers;
        gestionnaire->entrees->mouse_y = mouse_y_univers;
    }
}