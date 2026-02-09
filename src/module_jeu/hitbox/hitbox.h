/*
 * Moteur physique 2D.
 * Gere les collisions pour les jeux de plateforme et vue de dessus.
 */

#ifndef HITBOX_H
#define HITBOX_H

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
    float width;
    float height;
    float gravity;    /* Force de gravite */
    float powerJump;  /* Force de saut */
    bool inSky;       /* En l'air */
    bool requestJump; /* Demande de saut */
    bool leftLock;    /* Bloque a gauche */
    bool rightLock;   /* Bloque a droite */
} EntityPlatformer;

/* Entite pour la physique vue de dessus */
typedef struct {
    float x;
    float y;
    float width;
    float height;
} EntityTopdown;

/* Verifie si un type de bloc doit etre ignore */
bool est_type_ignorer(int type, int *types_a_ignorer, int nb_types);

/* Calcule les collisions pour un jeu de plateforme */
EntityPlatformer *hitbox_platforme(EntityPlatformer *entite, Blocks *blocs, float vitesse_max_chute,
                                   float correction_mur, int *types_ignorer, int nb_ignorer);

/* Calcule les collisions pour un jeu vue de dessus */
EntityTopdown *hitbox_topdown(EntityTopdown *entity, Blocks *blocks, int *types_ignorer,
                              int nb_ignorer);

/* Cree une entite de plateforme */
EntityPlatformer *creer_entite_platforme(float x, float y, float w, float h, float force_saut,
                                         float gravite);

/* Cree une entite vue de dessus */
EntityTopdown *creer_entite_topdown(float x, float y, float w, float h);

/* Libere une entite de plateforme */
void liberer_entite_platforme(EntityPlatformer *entite);

/* Libere une entite vue de dessus */
void liberer_entite_topdown(EntityTopdown *entite);

#ifdef __cplusplus
}
#endif

#endif /* HITBOX_H */