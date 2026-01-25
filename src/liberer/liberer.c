#include "../main.h"
#include <stdlib.h>

/*
 * Libère le tableau dynamique contenant les images de la frame actuelle.
 * Remet les compteurs et pointeurs à zéro.
 */
void free_tab_images(void) {
    if (!gs || !gs->image)
        goto gsvide;

    if (gs->image->tab) {
        xfree(gs->image->tab);
        gs->image->tab = NULL;
    }

    gs->image->nb_images = 0;
    gs->image->capacite_images = 0;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in free image table function");
}

/*
  Libère les ressources audio (Chunks) et le tableau du gestionnaire de son.
*/
void liberer_gestionnaire_son(void) {
    if (!gs)
        goto gsvide;

    GestionnaireSon *sons = gs->sons;

    for (int i = 0; i < sons->taille; i++) {
        if (sons->entrees[i].son) {
            Mix_FreeChunk(sons->entrees[i].son);
            sons->entrees[i].son = NULL;
        }
    }

    if (sons->entrees) {
        xfree(sons->entrees);
        sons->entrees = NULL;
    }

    sons->taille = 0;
    sons->capacite = 0;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in free sound manager function");
}

/*
  Libère les textures SDL et les listes de rotations associées.
*/
void liberer_gestionnaire_image(void) {
    if (!gs)
        goto gsvide;

    GestionnaireTextures *textures = gs->textures;

    for (int i = 0; i < textures->taille; i++) {
        if (textures->entrees[i].textures) {
            liberer_liste(textures->entrees[i].textures);
            textures->entrees[i].textures = NULL;
        }
    }

    if (textures->entrees) {
        xfree(textures->entrees);
        textures->entrees = NULL;
    }

    textures->taille = 0;
    textures->capacite = 0;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in free image manager function");
}

/*
  Libère le GestionnaireTiming.
*/
void liberer_timing(void) {
    if (!gs || !gs->timing)
        return;

    xfree(gs->timing);
    gs->timing = NULL;
}

/*
  Libère le GestionnaireFenetre et ses ressources SDL.
*/
void liberer_fenetre(void) {
    if (!gs || !gs->fenetre)
        return;

    GestionnaireFenetre *f = gs->fenetre;

    if (f->rendu) {
        SDL_DestroyRenderer(f->rendu);
        f->rendu = NULL;
    }

    if (f->fenetre) {
        SDL_DestroyWindow(f->fenetre);
        f->fenetre = NULL;
    }

    xfree(gs->fenetre);
    gs->fenetre = NULL;
}

/*
  Libère le GestionnaireEntrees.
*/
void liberer_entrees(void) {
    if (!gs || !gs->entrees)
        return;

    xfree(gs->entrees);
    gs->entrees = NULL;
}

/*
  Libère le fond_actualiser.
*/
void liberer_fond(void) {
    if (!gs || !gs->fond)
        return;

    xfree(gs->fond);
    gs->fond = NULL;
}

/*
  Libère le Tableau_image.
*/
void liberer_tableau_image(void) {
    if (!gs || !gs->image)
        return;

    free_tab_images();
    xfree(gs->image);
    gs->image = NULL;
}

/*
  Libère le GestionnaireTextures.
*/
void liberer_textures(void) {
    if (!gs || !gs->textures)
        return;

    liberer_gestionnaire_image();
    xfree(gs->textures);
    gs->textures = NULL;
}

/*
  Libère le GestionnaireSon.
*/
void liberer_sons(void) {
    if (!gs || !gs->sons)
        return;

    liberer_gestionnaire_son();
    xfree(gs->sons);
    gs->sons = NULL;
}

/*
  Libère les contrôleurs (manette et joystick).
*/
void liberer_manette(void) {
    if (!gs)
        return;
    for (int i = 0; i < NB_MANETTES_MAX; i++) {
        if (gs->entrees->man[i].manette != NULL) {
            fermer_controller(i);
            gs->entrees->man[i].manette = NULL;
        }

        if (gs->entrees->man[i].joystick) {
            fermer_joystick(i);
            gs->entrees->man[i].joystick = NULL;
        }
    }
}

void liberer_blocks(Blocks *ptr) {
    if (!ptr)
        return;

    xfree(ptr->tab);
    ptr->tab = NULL;

    xfree(ptr);
}

/*
  Fonction principale de destruction du moteur.
  Libère toute la mémoire allouée et ferme les sous-systèmes SDL.
*/
void liberer_gs(void) {
    if (!gs)
        goto gsvide;
    log_message(NiveauLogInfo, "Freeing manager...");

    /* Libération de chaque sous-structure */
    liberer_tableau_image();
    liberer_textures();
    liberer_sons();
    liberer_manette();
    liberer_entrees();
    liberer_fenetre();
    liberer_fond();
    liberer_timing();
    log_message(NiveauLogInfo, "manager freed successfully.");

    /* Fermeture des sous-systèmes SDL */
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    /* Libération du gestionnaire principal */
    xfree(gs);
    gs = NULL;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager NULL, nothing to free");
}