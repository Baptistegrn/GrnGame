/*
 * Liste chainee pour le cache des rotations de textures.
 */

#ifndef LISTE_CHAINE_H
#define LISTE_CHAINE_H

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Liste {
    SDL_Texture *texture;  /* texture */
    int angle;             /* angle de rotation de recherche */
    struct Liste *suivant; /* element suivant */
} Liste;

/* Cree une liste avec une texture et un angle */
Liste *creer_liste(SDL_Texture *valeur, int angle);

/* Ajoute une texture en tete de liste */
Liste *ajouter_en_tete(Liste *liste, SDL_Texture *valeur, int angle);

/* Libere toute la liste et ses textures */
Liste *liberer_liste(Liste *liste);

/* Recupere une texture par son angle */
SDL_Texture *recuperer_texture_angle_indice(Liste *liste, int angle);

#ifdef __cplusplus
}
#endif

#endif