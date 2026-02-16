/*
 * Initialisation du moteur de jeu GrnGame.
 * Configure SDL, les graphiques, le son, et charge les ressources.
 */

#include "../../allouer/allouer.h"
#include "../../chemin/chemin.h"
#include "../../main.h"
#include "../boucle/boucle.h"
#include "../fenetre/fenetre.h"
#include "../image/affichage/affichage.h"
#include "../image/ajout/ajout.h"
#include "../image/chargement/chargement_image.h"
#include "../json/json.h"
#include "../logging/logging.h"
#include "../son/son.h"
#include "initialisation.h"
#include "mapping_data.h"
#include <stdlib.h>
#include <string.h>


Gestionnaire *gs = NULL;

/*
 * Initialise tout le moteur de jeu : SDL, images, sons, fenetre, textures.
 * Valide les parametres, alloue les structures, initialise les bibliotheques
 * graphiques et audio, puis charge les ressources (images et sons).
 */
Gestionnaire *initialiser(int hauteur_univers, int largeur_univers, float fps, bool bande_noir,
                          const char *nom_fenetre) {
    /* Parametres par defaut */
    if (hauteur_univers <= 0 || largeur_univers <= 0) {
        log_fmt(NiveauLogErreur, "Invalid dimensions %dx%d, using 160x160", largeur_univers,
                hauteur_univers);
        hauteur_univers = largeur_univers = 160;
    }

    if (fps <= 0.0f) {
        log_message(NiveauLogErreur, "Invalid fps, using 60.0f");
        fps = 60.0f;
    }

    if (!nom_fenetre) {
        log_message(NiveauLogErreur, "Invalid window name, using default");
        nom_fenetre = "Game Window";
    }

    /* Allocation structures (fatal si echec) */
    gs = allouer_structures();
    creer_gestionnaire_fichiers();
    /* Configuration interne */
    gs->timing->run = true;
    gs->timing->fps = fps;
    gs->timing->en_pause = false;
    gs->timing->minimise = false;
    gs->timing->dt = 0.0001;
    gs->fenetre->hauteur_univers = hauteur_univers;
    gs->fenetre->largeur_univers = largeur_univers;
    gs->fenetre->nom_fenetre = nom_fenetre;

    gs->frame->fond->bande_noir = bande_noir;

    /* initialise les cles de chiffrement a 0 */
    mettre_cle_vide();

    /* Initialisation SDL (fatal si echec) */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        log_fmt(NiveauLogErreur, "SDL_Init failed: %s", SDL_GetError());
        /* Si echec on passe sans drivers */
#ifdef _WIN32
        _putenv("SDL_VIDEODRIVER=dummy");
#else
        setenv("SDL_VIDEODRIVER", "dummy", 1);
#endif

        /* On retente */
        if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
            log_fmt(NiveauLogErreur, "SDL_Init Fatal: %s", SDL_GetError());
#ifndef DEBUG_MODE
            exit(EXIT_FAILURE);
#endif
        }
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        log_fmt(NiveauLogErreur, "IMG_Init PNG failed: %s", IMG_GetError());
#ifndef DEBUG_MODE
        exit(EXIT_FAILURE);
#endif
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        log_fmt(NiveauLogErreur, "Mix_OpenAudio failed: %s", Mix_GetError());
#ifndef DEBUG_MODE
        exit(EXIT_FAILURE);
#endif
    }

    Mix_AllocateChannels(TAILLE_CANAL);

    /* chargement de la base de donnees de manettes */
    SDL_RWops *rw = SDL_RWFromMem(gamecontrollerdb_txt, gamecontrollerdb_txt_len);
    /* chargement depuis un fichier header */
    int mapping_compteur = SDL_GameControllerAddMappingsFromRW(rw, 1);
    if (mapping_compteur < 0) {
        log_fmt(NiveauLogErreur, "Impossible to map controllers : %s", SDL_GetError());
    } else {
        log_fmt(NiveauLogInfo, "%d mappings controllers charging", mapping_compteur);
    }
    /* Initialisation des sous-systemes */
    gs->frame->image->capacite_images = 10;
    gs->frame->image->nb_images = 0;
    gs->frame->image->tab =
        malloc_gestion_echec_compteur(sizeof(ObjectImage) * gs->frame->image->capacite_images);
    memset(gs->frame->image->tab, 0, sizeof(ObjectImage) * gs->frame->image->capacite_images);

    /* Fenetre (fatal si echec) */
    initialiser_fenetre();

    if (!gs->timing->run) {
        log_message(NiveauLogErreur, "Window initialization failed");
        exit(EXIT_FAILURE);
    }

    /* Ressources secondaires */
    init_gestionnaire_textures();
    init_gestionnaire_son();

    /* pour le hot reload en debug*/
#ifdef DEBUG_MODE
    gs->frame->fichiers_lua = renvoie_fichier_dossier("./src", "lua", NULL);
    for (int i = 0; i < gs->frame->fichiers_lua->taille; i++) {
        log_fmt(NiveauLogDebug, "file lua detected :%s", gs->frame->fichiers_lua->noms[i]);
    }
#endif

    log_message(NiveauLogDebug, "Initialization successful");
    return gs;
}
