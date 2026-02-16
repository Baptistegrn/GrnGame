/*
 * Liste chainee pour le cache des rotations de textures.
 */

#include "liste_chaine.h"
#include "../../../allouer/allouer.h"

/* Cree un nouveau noeud de liste chainee pour stocker une texture et son angle */
Liste *creer_liste(SDL_Texture *valeur, int angle) {
    Liste *nouv = malloc_gestion_echec_compteur(sizeof(Liste));
    nouv->texture = valeur;
    nouv->angle = angle;
    nouv->suivant = NULL;

    return nouv;
}

/* Ajoute une nouvelle texture au debut de la liste existante */
Liste *ajouter_en_tete(Liste *liste, SDL_Texture *valeur, int angle) {
    Liste *nouv = creer_liste(valeur, angle);

    if (!nouv)
        return liste;

    nouv->suivant = liste;

    return nouv;
}

/* Parcourt la liste chainee pour trouver la texture correspondant a l'angle */
SDL_Texture *recuperer_texture_angle_indice(Liste *liste, int angle) {
    Liste *courant = liste;

    while (courant != NULL) {
        if (courant->angle == angle) {
            return courant->texture;
        }
        courant = courant->suivant;
    }

    return NULL;
}

/* Libere la memoire de toute la liste chainee et detruit les textures SDL */
Liste *liberer_liste(Liste *liste) {
    Liste *courant = liste;

    while (courant != NULL) {
        Liste *temp = courant;
        courant = courant->suivant;

        if (temp->texture) {
            SDL_DestroyTexture(temp->texture);
        }

        free_gestion_echec_compteur(temp);
    }

    return NULL;
}