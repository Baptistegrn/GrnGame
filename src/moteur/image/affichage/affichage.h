/*
 * Affichage et rendu des images et objets.
 */

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* inclusion pour les types */
#include "../ajout/ajout.h"

typedef struct TableauImage {
    ObjectImage *tab;    /* tableau d'objets a afficher */
    int nb_images;       /* nombre d'objets dans le tableau */
    int capacite_images; /* capacite du tableau */
} TableauImage;

void actualiser(void);

void afficher_images(void);

void afficher_objet(ObjectImage *obj, SDL_Rect *dst, Sint16 x_ecran, Sint16 y_ecran, Sint16 w_ecran,
                    Sint16 h_ecran, unsigned char coeff);

bool hors_ecran(float posx, float posy, Sint16 taillex, Sint16 tailley, int decalage_x_scaled,
                int decalage_y_scaled);
void dessiner_particules(Particule *particules);

/* Fonctions de liberation */
void free_tab_images(void);
void liberer_gestionnaire_image(void);
void liberer_tableau_image(void);
void liberer_textures(void);

#ifdef __cplusplus
}
#endif

#endif /* AFFICHAGE_H */
