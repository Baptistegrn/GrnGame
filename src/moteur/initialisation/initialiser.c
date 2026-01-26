#include "../../main.h"

Gestionnaire *gs = NULL;

/*
 * Initialise tout le moteur de jeu : SDL, images, sons, fenêtre, textures.
 * Valide les paramètres, alloue les structures, initialise les bibliothèques
 * graphiques et audio, puis charge les ressources (images et sons).
 * Retourne un pointeur vers le Gestionnaire ou quitte le programme en cas d'erreur.
 */
Gestionnaire *initialiser(int hauteur_univers, int largeur_univers, float fps, bool bande_noir,
                          const char *nom_fenetre) {
    /* Paramètres par défaut */
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

    /* Allocation structures (fatal si échec) */
    gs = allouer_structures();

    /* Configuration interne */
    gs->timing->run = true;
    gs->timing->fps = fps;

    gs->fenetre->hauteur_univers = hauteur_univers;
    gs->fenetre->largeur_univers = largeur_univers;
    gs->fenetre->nom_fenetre = nom_fenetre;

    gs->fond->bande_noir = bande_noir;

    /* Initialisation SDL (fatal si échec) */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        log_fmt(NiveauLogErreur, "SDL_Init failed: %s", SDL_GetError());
        // si echec on passe sans drivers
#ifdef _WIN32
        _putenv("SDL_VIDEODRIVER=dummy");
#else
        setenv("SDL_VIDEODRIVER", "dummy", 1);
#endif

        // on retente
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
    int mapping_count = SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
    if (mapping_count < 0) {
        log_fmt(NiveauLogErreur, "Impossible to map controllers : %s", SDL_GetError());
    } else {
        log_fmt(NiveauLogInfo, "%d mappings controllers charging", mapping_count);
    }
    /* Initialisation des sous-systèmes */
    gs->image->capacite_images = 10;
    gs->image->nb_images = 0;
    gs->image->tab = xmalloc(sizeof(ObjectImage) * gs->image->capacite_images);
    memset(gs->image->tab, 0, sizeof(ObjectImage) * gs->image->capacite_images);

    /* Fenêtre (fatal si échec) */
    initialiser_fenetre();

    if (!gs->timing->run) {
        log_message(NiveauLogErreur, "Window initialization failed");
        exit(EXIT_FAILURE);
    }

    /* Ressources secondaires */
    init_gestionnaire_textures();
    init_gestionnaire_son();

    log_message(NiveauLogDebug, "Initialization successful");
    return gs;
}
