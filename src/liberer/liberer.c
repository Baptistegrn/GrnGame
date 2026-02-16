/*
 * Liberation principale du gestionnaire.
 * Appelle les fonctions de liberation de chaque module puis ferme SDL.
 */

#include "liberer.h"
#include "../allouer/allouer.h"
#include "../chemin/chemin.h"

#include "../moteur/boucle/boucle.h"

#include "../moteur/entrees/entrees.h"
#include "../moteur/fenetre/fenetre.h"
#include "../moteur/image/affichage/affichage.h"

#include "../bindings_lua/bindings_lua.h"
#include "../main.h"
#include "../moteur/image/chargement/chargement_image.h"
#include "../moteur/image/dessin_forme/dessin.h"
#include "../moteur/image/liste/liste_chaine.h"
#include "../moteur/image/rotation/rotation.h"
#include "../moteur/initialisation/initialisation.h"
#include "../moteur/json/json.h"
#include "../moteur/logging/logging.h"
#include "../moteur/son/son.h"

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