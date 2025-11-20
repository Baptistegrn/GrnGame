#include "main.h"
#include "logging.h" // Ajout pour log_fmt

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int fenetre_init(Gestionnaire* gestionnaire, const char* nom_fenetre)
{
    if (!gestionnaire)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire NULL lors de l'initialisation de la fenêtre");
        return 1;
    }

    if (!nom_fenetre)
    {
        log_fmt(NiveauLogErreur, "Erreur: Nom de fenêtre NULL");
        return 1;
    }

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'initialiser SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_ShowCursor(SDL_DISABLE);

    // Initialisation de SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'initialiser SDL_image (PNG): %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Calcul des dimensions de la fenêtre
    gestionnaire->largeur_actuel = gestionnaire->largeur * gestionnaire->coeff_minimise;
    gestionnaire->hauteur_actuel = gestionnaire->hauteur * gestionnaire->coeff_minimise;
    gestionnaire->plein_ecran = false;

    // Création de la fenêtre
    gestionnaire->fenetre =
        SDL_CreateWindow(nom_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gestionnaire->largeur_actuel,
                         gestionnaire->hauteur_actuel, SDL_WINDOW_SHOWN);

    if (!gestionnaire->fenetre)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible de créer la fenêtre: %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Création du renderer avec accélération matérielle
    Uint32 flags = SDL_RENDERER_ACCELERATED;
    gestionnaire->rendu = SDL_CreateRenderer(gestionnaire->fenetre, -1, flags);

    if (!gestionnaire->rendu)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible de créer le renderer: %s", SDL_GetError());
        SDL_DestroyWindow(gestionnaire->fenetre);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Configuration du rendu pixel-perfect
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetIntegerScale(gestionnaire->rendu, SDL_TRUE);

    // Assignation du renderer au gestionnaire de textures
    if (gestionnaire->textures)
    {
        gestionnaire->textures->rendu = gestionnaire->rendu;
    }


    return 0;
}

void colorier(Gestionnaire* jeu, int r, int g, int b)
{
    if (!jeu)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire NULL dans colorier()");
        return;
    }
    fond_actualiser *fond = jeu->fond;
    if (!fond)
    {
        log_fmt(NiveauLogErreur, "Erreur: Configuration du fond non initialisée");
        return;
    }

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
        log_fmt(NiveauLogErreur, "Erreur: Valeurs RGB invalides (%d, %d, %d). Valeurs attendues: 0-255", r, g, b);
        return;
    }
    fond->r = r;
    fond->g = g;
    fond->b = b;
    fond->colorier_frame = true;

}