#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "logging.h"
#include "main.h"

#define TAILLE_CANAL 32

Gestionnaire* initialisation(int hauteur, int largeur, float fps, int coeff, char* lien_image, char* lien_son,
                            bool bande_noir, const char* nom_fenetre, const char *chemin_log)
{
    if (chemin_log != NULL) {
        initialiser_logging(DestinationLogFichier, chemin_log, NiveauLogAvertissement);
    } else {
        initialiser_logging(DestinationLogConsole, NULL, NiveauLogAvertissement);
    }


    // Validation des paramètres
    if (hauteur <= 0 || largeur <= 0)
    {
        fprintf(stderr, "Erreur: Dimensions invalides (%dx%d)\n", largeur, hauteur);
        return NULL;
    }

    if (fps <= 0)
    {
        fprintf(stderr, "Erreur: FPS invalide (%f)\n", fps);
        return NULL;
    }

    if (!nom_fenetre)
    {
        fprintf(stderr, "Erreur: Nom de fenêtre NULL\n");
        return NULL;
    }

    // Allocation du gestionnaire principal
    Gestionnaire* jeu = (Gestionnaire*)malloc(sizeof(Gestionnaire));
    if (!jeu)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour le gestionnaire principal\n");
        return NULL;
    }
    memset(jeu, 0, sizeof(Gestionnaire));

    // Configuration des paramètres principaux
    jeu->run = true;
    jeu->fps = fps;
    jeu->hauteur = hauteur;
    jeu->largeur = largeur;
    jeu->coeff_minimise = coeff;
    jeu->controller = NULL;
    jeu->joystick = NULL;

    // Allocation des sous-structures
    jeu->fond = (fond_actualiser*)malloc(sizeof(fond_actualiser));
    jeu->entrees = (GestionnaireEntrees*)malloc(sizeof(GestionnaireEntrees));
    jeu->image = (Tableau_image*)malloc(sizeof(Tableau_image));
    jeu->textures = (GestionnaireTextures*)malloc(sizeof(GestionnaireTextures));
    jeu->sons = (GestionnaireSon*)malloc(sizeof(GestionnaireSon));

    if (!jeu->fond || !jeu->entrees || !jeu->image || !jeu->textures || !jeu->sons)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour les sous-structures\n");
        free_gestionnaire(jeu);
        return NULL;
    }
    // eviter probleme de memoire
    memset(jeu->fond, 0, sizeof(fond_actualiser));
    memset(jeu->entrees, 0, sizeof(GestionnaireEntrees));
    memset(jeu->image, 0, sizeof(Tableau_image));
    memset(jeu->textures, 0, sizeof(GestionnaireTextures));
    memset(jeu->sons, 0, sizeof(GestionnaireSon));

    // Configuration du fond d'écran
    jeu->fond->bande_noir = bande_noir;

    // Initialisation du tableau d'images
    jeu->image->capacite_images = 10;
    jeu->image->nb_images = 0;
    jeu->image->tab = (image*)malloc(sizeof(image) * jeu->image->capacite_images);
    if (!jeu->image->tab)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour le tableau d'images\n");
        free_gestionnaire(jeu);
        return NULL;
    }
    memset(jeu->image->tab, 0, sizeof(image) * jeu->image->capacite_images);

    // Initialisation de la fenêtre SDL
    if (fenetre_init(jeu, nom_fenetre) != 0)
    {
        fprintf(stderr, "Erreur: Échec de l'initialisation de la fenêtre\n");
        free_gestionnaire(jeu);
        return NULL;
    }

    // Initialisation de SDL_image
    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags)
    {
        fprintf(stderr, "Erreur: Impossible d'initialiser SDL_image: %s\n", IMG_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }

    // Initialisation du sous-système de contrôleurs
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        fprintf(stderr, "Erreur: Impossible d'initialiser SDL_GAMECONTROLLER: %s\n", SDL_GetError());
    }

    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
    {
        fprintf(stderr, "Erreur: Impossible d'initialiser SDL_JOYSTICK: %s\n", SDL_GetError());
    }

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur: Impossible d'initialiser SDL_mixer: %s\n", Mix_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }

    // Allocation des canaux audio
    Mix_AllocateChannels(TAILLE_CANAL);

    // Initialisation des gestionnaires de ressources
    if (!jeu->rendu)
    {
        fprintf(stderr, "Erreur: Renderer non initialisé\n");
        free_gestionnaire(jeu);
        return NULL;
    }

    init_gestionnaire_textures(jeu->textures, jeu->rendu);
    init_gestionnaire_son(jeu->sons);

    // Chargement des ressources

    if (lien_image)
    {   
        normaliser_chemin(lien_image);
        charger_toutes_les_textures(jeu->textures, lien_image);
    }

    if (lien_son)
    {
        normaliser_chemin(lien_son);
        charger_tous_les_sons(jeu->sons, lien_son);
    }


    return jeu;
}
