#include "main.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool init_controller_joysticks(Gestionnaire* jeu, int index)
{
    if (!jeu)
    {
        fprintf(stderr, "Erreur: Gestionnaire de jeu NULL lors de l'initialisation du contrôleur\n");
        return false;
    }

    if (SDL_NumJoysticks() <= index)
    {
        fprintf(stderr, "Erreur: Aucune manette disponible à l'index %d\n", index);
        return false;
    }

    if (!SDL_IsGameController(index))
    {
        fprintf(stderr, "Erreur: L'appareil %d n'est pas une manette reconnue\n", index);
        return false;
    }

    SDL_GameController* controller = SDL_GameControllerOpen(index);
    if (!controller)
    {
        fprintf(stderr, "Erreur: Impossible d'ouvrir la manette %d: %s\n", index, SDL_GetError());
        return false;
    }

    SDL_Joystick* joy = SDL_JoystickOpen(index);
    if (!joy)
    {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le joystick %d: %s\n", index, SDL_GetError());
        SDL_GameControllerClose(controller);
        return false;
    }

    jeu->controller = controller;
    jeu->joystick = joy;

    // Log de confirmation
    fprintf(stderr, "Manette %d ouverte: %s\n", index, SDL_GameControllerName(controller));
    return true;
}

void fermer_controller(Gestionnaire* jeu)
{
    if (!jeu)
    {
        return;
    }

    if (jeu->controller)
    {
        SDL_GameControllerClose(jeu->controller);
        jeu->controller = NULL;
    }
}

void fermer_joystick(Gestionnaire* jeu)
{
    if (!jeu)
    {
        return;
    }

    if (jeu->joystick)
    {
        SDL_JoystickClose(jeu->joystick);
        jeu->joystick = NULL;
    }
}

float* renvoie_joysticks(GestionnaireEntrees* entrees, float dead_zone)
{
    if (!entrees)
    {
        fprintf(stderr, "Erreur: Gestionnaire d'entrées NULL\n");
        return NULL;
    }

    float* valeurs = malloc(sizeof(float) * 6);
    if (!valeurs)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour les valeurs du joystick\n");
        return NULL;
    }

    // Normalisation des valeurs (-1.0 à 1.0)
    const float normalisation = 32766.0f;

    // Stick gauche
    valeurs[0] = (float)entrees->Joy.left.x / normalisation;
    valeurs[1] = (float)entrees->Joy.left.y / normalisation;

    // Stick droit
    valeurs[2] = (float)entrees->Joy.right.x / normalisation;
    valeurs[3] = (float)entrees->Joy.right.y / normalisation;

    // Triggers
    valeurs[4] = (float)entrees->trigger.triggerleft / normalisation;
    valeurs[5] = (float)entrees->trigger.triggerright / normalisation;

    // Application de la zone morte
    for (int i = 0; i < 6; i++)
    {
        if (fabsf(valeurs[i]) < dead_zone)
        {
            valeurs[i] = 0.0f;
        }
    }

    return valeurs;
}
