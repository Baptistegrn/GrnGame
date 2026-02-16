/*
 * Liberation des ressources liees aux images et textures.
 */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../boucle/boucle.h"
#include "../fenetre/fenetre.h"
#include "../image/affichage/affichage.h"
#include "../image/ajout/ajout.h"
#include "../image/chargement/chargement_image.h"
#include "../logging/logging.h"

/* Libere le tableau dynamique contenant les images de la frame actuelle */
void free_tab_images(void) {
    if (!gs || !gs->frame->image)
        goto gsvide;

    if (gs->frame->image->tab) {
        free_gestion_echec_compteur(gs->frame->image->tab);
        gs->frame->image->tab = NULL;
    }

    gs->frame->image->nb_images = 0;
    gs->frame->image->capacite_images = 0;
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in free image table function");
}

/* Libere les textures SDL et les listes de rotations associees */
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
        free_gestion_echec_compteur(textures->entrees);
        textures->entrees = NULL;
    }

    textures->taille = 0;
    textures->capacite = 0;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in free image manager function");
}

/* Libere le Tableau_image */
void liberer_tableau_image(void) {
    if (!gs || !gs->frame->image)
        return;

    free_tab_images();
    free_gestion_echec_compteur(gs->frame->image);
    gs->frame->image = NULL;
}

/* Libere le GestionnaireTextures */
void liberer_textures(void) {
    if (!gs || !gs->textures)
        return;

    liberer_gestionnaire_image();
    free_gestion_echec_compteur(gs->textures);
    gs->textures = NULL;
}

/* libere le sprite */
void liberer_sprite(Sprite *sprite) {
    if (sprite) {
        free_gestion_echec_compteur(sprite);
    }
}