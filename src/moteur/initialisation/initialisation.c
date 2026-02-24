/*
 * Initialisation du moteur de jeu GrnGame.
 */

#include "initialisation.h"
#include "../../allouer/allouer.h"
#include "../../chemin/chemin.h"
#include "../../main.h"
#include "../../module_jeu/module_jeu.h"
#include "../boucle/boucle.h"
#include "../entrees/entrees.h"
#include "../fenetre/fenetre.h"
#include "../image/chargement/chargement_image.h"
#include "../json/json.h"
#include "../logging/logging.h"
#include "../son/son.h"
#include "mapping_data.h"
#include <stdlib.h>

/*
 * Alloue toutes les structures, initialise SDL/IMG/Mixer,
 * et cree la fenetre + le renderer.
 * A appeler en premier, avant tout chargement d'assets.
 */
void initialisation(void) {
    initialiser_gestionnaire();
    initialiser_gestionnaire_fichiers();
    initialiser_gestionnaire_entrees();
    initialiser_gestionnaire_frame();
    initialiser_gestionnaire_son();
    initialiser_gestionnaire_timing();
    initialiser_gestionnaire_textures();
    initialiser_gestionnaire_fenetre();
    /* SDL / IMG / Mixer */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        log_fmt(NiveauLogErreur, "SDL_Init failed: %s", SDL_GetError());
        SDL_setenv("SDL_VIDEODRIVER", "dummy", 1);
        if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
            log_fmt(NiveauLogErreur, "SDL_Init Fatal: %s", SDL_GetError());
#ifndef DEBUG_MODE
            exit(EXIT_FAILURE);
#endif
        }
    }
    /* sdl doit etre initialiser avant le thread de hitbox */
    initialiser_gestionnaire_module_jeu();

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

    /* Base de donnees manettes */
    SDL_RWops *rw = SDL_RWFromMem(gamecontrollerdb_txt, gamecontrollerdb_txt_len);
    int mapping_compteur = SDL_GameControllerAddMappingsFromRW(rw, 1);
    if (mapping_compteur < 0) {
        log_fmt(NiveauLogErreur, "Impossible to map controllers: %s", SDL_GetError());
    } else {
        log_fmt(NiveauLogInfo, "%d mappings controllers charging", mapping_compteur);
    }

    /* Fenetre + renderer — necessaire avant tout chargement de texture */
    initialiser_fenetre();

    if (!gs->timing->run) {
        log_message(NiveauLogErreur, "Window initialization failed");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG_MODE
    gs->frame->fichiers_lua = renvoie_fichier_dossier("./src", "lua", NULL);
    for (int i = 0; i < gs->frame->fichiers_lua->taille; i++) {
        log_fmt(NiveauLogDebug, "file lua detected: %s", gs->frame->fichiers_lua->noms[i]);
    }
#endif

    log_message(NiveauLogErreur, "initialization of structures sucesfull");
}
