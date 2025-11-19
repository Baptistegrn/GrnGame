#include "main.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>


void redimensionner_fenetre(Gestionnaire* gestionnaire)
{
    if (!gestionnaire)
    {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans redimensionner_fenetre()\n");
        return;
    }

    if (!gestionnaire->fenetre || !gestionnaire->rendu)
    {
        fprintf(stderr, "Erreur: Fenêtre ou renderer NULL\n");
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
        fprintf(stderr, "Erreur: Impossible d'obtenir l'index de l'écran: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect display_bounds;
    if (SDL_GetDisplayBounds(display_index, &display_bounds) != 0)
    {
        fprintf(stderr, "Erreur: Impossible d'obtenir les dimensions de l'écran: %s\n", SDL_GetError());
        return;
    }

    SDL_DisplayMode mode;
    if (SDL_GetCurrentDisplayMode(display_index, &mode) != 0)
    {
        fprintf(stderr, "Erreur: Impossible d'obtenir le mode d'affichage: %s\n", SDL_GetError());
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
        // Passage en mode fenêtré
        nouveau_decalage_x = 0;
        nouveau_decalage_y = 0;
        nouvelle_largeur = largeur_base * coeff_minimise;
        nouvelle_hauteur = hauteur_base * coeff_minimise;

        SDL_SetWindowFullscreen(fenetre, 0);
        SDL_SetWindowSize(fenetre, nouvelle_largeur, nouvelle_hauteur);
        SDL_SetWindowPosition(fenetre, display_bounds.x + (mode.w - nouvelle_largeur) / 2,
                              display_bounds.y + (mode.h - nouvelle_hauteur) / 2);
        SDL_SetWindowBordered(fenetre, SDL_TRUE);
    }
    else
    {
        // Passage en plein écran avec coefficient par pas de 0.5
        float coeff_largeur_f = (float)mode.w / (float)largeur_base;
        float coeff_hauteur_f = (float)mode.h / (float)hauteur_base;
        float coeff_min = (coeff_largeur_f < coeff_hauteur_f) ? coeff_largeur_f : coeff_hauteur_f;

        // Arrondir au multiple de 0.5 inférieur
        float coeff = floorf(coeff_min * 2.0f) / 2.0f;

        nouvelle_largeur = (int)(largeur_base * coeff);
        nouvelle_hauteur = (int)(hauteur_base * coeff);

        // Calcul du décalage pour centrer l'image (letterboxing/pillarboxing)
        nouveau_decalage_x = (mode.w - nouvelle_largeur) / 2;
        nouveau_decalage_y = (mode.h - nouvelle_hauteur) / 2;

        SDL_SetWindowFullscreen(fenetre, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    // Mise à jour de l'état du gestionnaire
    gestionnaire->largeur_actuel = nouvelle_largeur;
    gestionnaire->hauteur_actuel = nouvelle_hauteur;
    gestionnaire->decalage_x = nouveau_decalage_x;
    gestionnaire->decalage_y = nouveau_decalage_y;
    gestionnaire->plein_ecran = !plein_ecran;

    // Repositionnement de la souris pour maintenir sa position relative
    if (gestionnaire->entrees)
    {
        float coeff_largeur = (float)nouvelle_largeur / (float)largeur_base;
        float coeff_hauteur = (float)nouvelle_hauteur / (float)hauteur_base;

        int mouse_x_screen = (int)lroundf(mouse_x_univers * coeff_largeur + nouveau_decalage_x);
        int mouse_y_screen = (int)lroundf(mouse_y_univers * coeff_hauteur + nouveau_decalage_y);

        SDL_WarpMouseInWindow(fenetre, mouse_x_screen, mouse_y_screen);

        // Conservation de la position dans l'univers du jeu
        gestionnaire->entrees->mouse_x = mouse_x_univers;
        gestionnaire->entrees->mouse_y = mouse_y_univers;
    }

    return;
}