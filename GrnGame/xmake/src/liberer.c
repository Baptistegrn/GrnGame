#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void free_tab_images(Gestionnaire* gestionnaire)
{
    if (!gestionnaire || !gestionnaire->image)
    {
        fprintf(stderr, "ERREUR: Gestionnaire NULL et tableau dimages vide dans la liberation image\n");
        return;
    }

    free(gestionnaire->image->tab);
    gestionnaire->image->tab = NULL;
    gestionnaire->image->nb_images = 0;
    gestionnaire->image->capacite_images = 0;
}

void liberer_gestionnaire_son(GestionnaireSon* gs)
{
    if (!gs)
    {
        fprintf(stderr, "Impossible de libérer le son car le gestionnaire est NULL\n");
        return;
    }

    for (int i = 0; i < gs->taille; i++)
    {
        if (gs->entrees[i].son)
        {
            Mix_FreeChunk(gs->entrees[i].son);
            gs->entrees[i].son = NULL;
        }
    }

    if (gs->entrees)
    {
        free(gs->entrees);
        gs->entrees = NULL;
    }
    gs->taille = 0;
    gs->capacite = 0;
}

void liberer_gestionnaire_image(GestionnaireTextures* gs)
{
    if (!gs)
    {
        fprintf(stderr, "Impossible de libérer l'image car le gestionnaire est NULL\n");
        return;
    }

    for (int i = 0; i < gs->taille; i++)
    {
        if (gs->entrees[i].texture)
        {
            SDL_DestroyTexture(gs->entrees[i].texture);
            gs->entrees[i].texture = NULL;
        }
    }

    if (gs->entrees)
    {
        free(gs->entrees);
        gs->entrees = NULL;
    }
    gs->taille = 0;
    gs->capacite = 0;
}


void free_gestionnaire(Gestionnaire* jeu)
{
    if (!jeu)
        return;

    if (jeu->image)
    {
        free(jeu->image->tab);
        free(jeu->image);
    }
    if (jeu->fond)
        free(jeu->fond);
    if (jeu->entrees)
        free(jeu->entrees);
    if (jeu->textures)
        free(jeu->textures);
    if (jeu->sons)
        free(jeu->sons);

    free(jeu);
}

void liberer_jeu(Gestionnaire* jeu)
{
    if (!jeu)
    {
        fprintf(stderr, "DEBUG: le gerstionnaire principale est NULL donc impossible de le liberer\n");
        return;
    }

    free_tab_images(jeu);

    liberer_gestionnaire_image(jeu->textures);
    free(jeu->textures);

    liberer_gestionnaire_son(jeu->sons);
    free(jeu->sons);

    if (jeu->entrees)
        free(jeu->entrees);

    if (jeu->controller)
        fermer_controller(jeu);
    if (jeu->joystick)
        fermer_joystick(jeu);

    if (jeu->rendu)
        SDL_DestroyRenderer(jeu->rendu);
    if (jeu->fenetre)
        SDL_DestroyWindow(jeu->fenetre);

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    free(jeu);
}
