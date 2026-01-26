/*
 * Accesseurs pour les constantes globales du moteur.
 */

#ifndef CONSTANTE_H
#define CONSTANTE_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

int souris_x(void);
int souris_y(void);

int souris_gauche_juste_presse(void);
int souris_droite_juste_presse(void);

int souris_gauche_presse(void);
int souris_droite_presse(void);

int souris_scroll_vertical(void);
int souris_scroll_horizontal(void);

float dt(void);
float nb_images(void);

Uint32 compteur_frame(void);

int decalage_x(void);
int decalage_y(void);
int largeur_actuelle(void);
int hauteur_actuelle(void);
int largeur_univers(void);
int hauteur_univers(void);
int coeff(void);

#ifdef __cplusplus
}
#endif

#endif /* CONSTANTE_H */