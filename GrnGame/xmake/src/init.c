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
char* couper_extention(const char *texte) {
    const char *pos = strrchr(texte, '.');  

    size_t len;
    if (!pos) { 
        len = strlen(texte);
    } else {
        len = pos - texte;
    }

    char *resultat = malloc(len + 1);
    if (!resultat) return NULL;

    strncpy(resultat, texte, len);
    resultat[len] = '\0';
    return resultat;
}

Gestionnaire* initialisation(int hauteur, int largeur, float fps, int coeff, char* lien_image, char* lien_son,
                            bool bande_noir, const char* nom_fenetre, const char *chemin_log)

{

    char *sans_ext = couper_extention(chemin_log);

    const char *suffixe = "_python.logs";
    size_t taille = strlen(suffixe) + strlen(sans_ext) + 1;
    char *dest = malloc(taille);

    strcpy(dest, sans_ext);
    strcat(dest, suffixe);
    if (freopen(dest, "w", stderr) != NULL) {
        setvbuf(stderr, NULL, _IONBF, 0);
    } else {
        fprintf(stdout, "Attention: Impossible de créer %s\n",dest);
    }
    free(sans_ext);
    free(dest);

    if (chemin_log != NULL) {
        initialiser_logging(DestinationLogFichier, chemin_log, NiveauLogInfo);
    } else {
        initialiser_logging(DestinationLogConsole, NULL, NiveauLogInfo);
    }


    // Validation des paramètres
    if (hauteur <= 0 || largeur <= 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Dimensions invalides (%dx%d)", largeur, hauteur);
        return NULL;
    }

    if (fps <= 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: FPS invalide (%f)", fps);
        return NULL;
    }

    if (!nom_fenetre)
    {
        log_fmt(NiveauLogErreur, "Erreur: Nom de fenêtre NULL");
        return NULL;
    }

    // Allocation du gestionnaire principal
    Gestionnaire* jeu = (Gestionnaire*)malloc(sizeof(Gestionnaire));
    if (!jeu)
    {
        log_fmt(NiveauLogErreur, "Erreur: Échec d'allocation mémoire pour le gestionnaire principal");
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
        log_fmt(NiveauLogErreur, "Erreur: Échec d'allocation mémoire pour les sous-structures");
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
        log_fmt(NiveauLogErreur, "Erreur: Échec d'allocation mémoire pour le tableau d'images");
        free_gestionnaire(jeu);
        return NULL;
    }
    memset(jeu->image->tab, 0, sizeof(image) * jeu->image->capacite_images);

    // Initialisation de la fenêtre SDL
    if (fenetre_init(jeu, nom_fenetre) != 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Échec de l'initialisation de la fenêtre");
        free_gestionnaire(jeu);
        return NULL;
    }

    // Initialisation de SDL_image
    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'initialiser SDL_image: %s", IMG_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }

    // Initialisation du sous-système de contrôleurs
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'initialiser SDL_GAMECONTROLLER: %s", SDL_GetError());
    }

    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'initialiser SDL_JOYSTICK: %s", SDL_GetError());
    }

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'initialiser SDL_mixer: %s", Mix_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }

    // Allocation des canaux audio
    Mix_AllocateChannels(TAILLE_CANAL);

    // Initialisation des gestionnaires de ressources
    if (!jeu->rendu)
    {
        log_fmt(NiveauLogErreur, "Erreur: Renderer non initialisé");
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