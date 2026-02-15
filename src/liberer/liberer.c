/*
 * Liberation principale du gestionnaire.
 * Appelle les fonctions de liberation de chaque module puis ferme SDL.
 */

#include "../main.h"

/*
 * Libere toutes les ressources du moteur.
 * Appele a la fermeture du jeu ou en cas d'erreur fatale.
 */
void liberer_gs(void) {
    if (!gs)
        goto gsvide;

    log_message(NiveauLogInfo, "Freeing manager...");

    /* Liberation des sous-structures */
    liberer_tableau_image();
    liberer_textures();
    liberer_sons();
    liberer_manette();
    liberer_entrees();
    liberer_fenetre();
    liberer_fond();
    liberer_timing();
    liberer_fichiers_lua();
    liberer_gestionnaire_fichiers();
    /* Liberation du frame */
    if (gs->frame) {
        free_gestion_echec_compteur(gs->frame);
        gs->frame = NULL;
    }

    log_message(NiveauLogInfo, "manager freed successfully.");

    /* Fermeture des sous-systemes SDL */
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    /* Liberation du gestionnaire principal */
    free_gestion_echec_compteur(gs);
    gs = NULL;
    return;

gsvide:
    log_message(NiveauLogDebug, "manager NULL, nothing to free");
}