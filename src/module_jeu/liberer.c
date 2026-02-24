/* libere le module de jeu */

#include "../allouer/allouer.h"
#include "../main.h"
#include "../moteur/boucle/boucle.h"
#include "../moteur/fenetre/fenetre.h"
#include "../prediction_branche.h"
#include "camera/camera.h"
#include "module_jeu.h"

/* libere le gestionnaire de module de jeu */
void liberer_gestionnaire_module_jeu(void) {
    if (UNLIKELY(!gs->module_jeu))
        return;
    if (gs->module_jeu->thread_present) {
        SDL_LockMutex(gs->module_jeu->mutex);
        gs->module_jeu->thread_actif = false;
        SDL_CondSignal(gs->module_jeu->reveil);
        SDL_UnlockMutex(gs->module_jeu->mutex);

        SDL_WaitThread(gs->module_jeu->hitbox_t, NULL);
    }
    if (gs->module_jeu->mutex)
        SDL_DestroyMutex(gs->module_jeu->mutex);
    if (gs->module_jeu->reveil)
        SDL_DestroyCond(gs->module_jeu->reveil);
    if (gs->module_jeu->termine)
        SDL_DestroyCond(gs->module_jeu->termine);
    liberer_camera(gs->module_jeu->camera);
    free_gestion_echec_compteur(gs->module_jeu);
}