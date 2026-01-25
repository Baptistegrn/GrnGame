#include "../../../main.h"

/*
 * Crée un nouveau nœud de liste chaînée pour stocker une texture et son angle.
 * Alloue la mémoire et initialise les pointeurs.
 */
Liste *creer_liste(SDL_Texture *valeur, int angle) {
    Liste *nouv = xmalloc(sizeof(Liste));
    nouv->texture = valeur;
    nouv->angle = angle;
    nouv->suivant = NULL;

    return nouv;
}

/*
 * Ajoute une nouvelle texture au début de la liste existante.
 * Crée un nouveau nœud et le chaîne à l'ancienne tête de liste.
 */
Liste *ajouter_en_tete(Liste *liste, SDL_Texture *valeur, int angle) {
    Liste *nouv = creer_liste(valeur, angle);

    if (!nouv)
        return liste;

    nouv->suivant = liste;

    return nouv;
}

/*
 * Parcourt la liste chaînée pour trouver la texture correspondant à l'angle
 * donné. Retourne NULL si le couple texture-angle n'est pas trouvé.
 */
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

/*
 * Libère la mémoire de toute la liste chaînée et détruit les textures SDL
 * associées. Retourne NULL pour faciliter la remise à zéro du pointeur
 * appelant.
 */
Liste *liberer_liste(Liste *liste) {
    Liste *courant = liste;

    while (courant != NULL) {
        Liste *temp = courant;
        courant = courant->suivant;

        if (temp->texture) {
            SDL_DestroyTexture(temp->texture);
        }

        xfree(temp);
    }

    return NULL;
}