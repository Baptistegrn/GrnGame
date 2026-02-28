/* initialise le module de jeu */

#include "../allouer/allouer.h"
#include "../main.h"
#include "../moteur/boucle/boucle.h"
#include "../moteur/fenetre/fenetre.h"
#include "../moteur/logging/logging.h"
#include "../prediction_branche.h"
#include "camera/camera.h"
#include "hitbox/hitbox.h"
#include "module_jeu.h"
#include <string.h>

/* Fonction executee en boucle par le thread persistant */
static int thread_de_hitbox(void *arg) {
    GestionnaireModuleJeu *mod = (GestionnaireModuleJeu *)arg;

    /* verrou mutex */
    SDL_LockMutex(mod->mutex);

    while (mod->thread_actif) {
        while (!mod->travail_demande && mod->thread_actif) {
            SDL_CondWait(mod->reveil, mod->mutex);
        }

        if (!mod->thread_actif) {
            break;
        }

        /* deverouiller le thread */
        SDL_UnlockMutex(mod->mutex);

        /* appel de la hitbox */
        hitbox_platforme(mod->donnees.entites, mod->donnees.blocs, mod->donnees.masque_ignorer,
                         mod->donnees.dt);

        /* on libere  */
        SDL_LockMutex(mod->mutex);
        mod->travail_demande = false;
        mod->travail_termine = true;
        SDL_CondSignal(mod->termine);
    }

    SDL_UnlockMutex(mod->mutex);
    return 0;
}

/* initialise camera et thread */
void initialiser_gestionnaire_module_jeu(void) {
    gs->module_jeu = malloc_gestion_echec_compteur(sizeof(GestionnaireModuleJeu));
    memset(gs->module_jeu, 0, sizeof(GestionnaireModuleJeu));

    initialiser_camera();
    gs->module_jeu->mutex = SDL_CreateMutex();
    gs->module_jeu->reveil = SDL_CreateCond();
    gs->module_jeu->termine = SDL_CreateCond();
    gs->module_jeu->thread_actif = true;
    gs->module_jeu->travail_demande = false;
    gs->module_jeu->travail_termine = true;

    gs->module_jeu->hitbox_t = SDL_CreateThread(thread_de_hitbox, "thread_hitbox", gs->module_jeu);

    if (gs->module_jeu->hitbox_t != NULL) {
        gs->module_jeu->thread_present = true;
    } else {
        log_fmt(NiveauLogErreur, "error creation of thread : %s", SDL_GetError());
    }
}

void attendre_calcul_hitbox(void) {
    if (UNLIKELY(!gs->module_jeu->thread_present))
        return;

    SDL_LockMutex(gs->module_jeu->mutex);

    /* On attend que le travail soit fini */
    while (!gs->module_jeu->travail_termine) {
        SDL_CondWait(gs->module_jeu->termine, gs->module_jeu->mutex);
    }

    SDL_UnlockMutex(gs->module_jeu->mutex);
}
