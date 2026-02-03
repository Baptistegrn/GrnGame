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

Liste *creer_liste(SDL_Texture *valeur, int angle);
Liste *ajouter_en_tete(Liste *liste, SDL_Texture *valeur, int angle);
Liste *liberer_liste(Liste *liste);
SDL_Texture *recuperer_texture_angle_indice(Liste *liste, int angle);

#ifdef __cplusplus
}
#endif

#endif