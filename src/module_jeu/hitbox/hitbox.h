/*
 * Moteur physique 2D.
 * Gere les collisions pour les jeux de plateforme .
 */

#ifndef HITBOX_H
#define HITBOX_H

#include <SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "../blocs/bloc.h"

/* Entite pour la physique de plateforme avec gravite et saut */
typedef struct {
    float x;
    float y;
    float speedY; /* Vitesse verticale */
    float speedX; /* Vitesse horizontale */
    float width;
    float height;
    float gravity;        /* Force de gravite */
    float speedMaxX;      /* Vitesse entite horizontale max */
    float powerJump;      /* Force de saut */
    bool inSky;           /* En l'air */
    bool requestJump;     /* Demande de saut */
    bool requestLeft;     /* Demande de se deplacer a gauche */
    bool requestRight;    /* Demande de se deplacer a droite */
    bool leftLock;        /* Bloque a gauche */
    bool rightLock;       /* Bloque a droite */
    float wallCorrection; /* freine si coller a un mur */
    float speedMaxFall;   /* vitesse max de chute */
    float acceleration;   /* acceleration */
    float initialSpeed;   /* vitesse initiale x*/

} EntityPlatformer;

/* Calcule les collisions pour un jeu de plateforme */
EntityPlatformer *hitbox_platforme(EntityPlatformer *entite, Blocks *blocs, Uint32 masque_ignorer,
                                   float dt);

/* Cree une entite de plateforme */
EntityPlatformer *creer_entite_platforme(float x, float y, float w, float h, float force_saut,
                                         float gravite, float vitesse_x_max,
                                         float vitesse_max_chute, float correction_mur,
                                         float vitesse_initiale, float acceleration);

/* Libere une entite de plateforme */
void liberer_entite_platforme(EntityPlatformer *entite);

#ifdef __cplusplus
}
#endif

#endif /* HITBOX_H */