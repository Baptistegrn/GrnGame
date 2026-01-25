/*
 * Moteur physique 2D pour la plateforme.
 */

#ifndef HITBOX_H
#define HITBOX_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float x;
    float y;
    float w;
    float h;
    int type;
} Block;

typedef struct {
    Block *tab;
    int size;
    int capacity;
} Blocks;

typedef struct {
    float x;
    float y;
    float speedY;
    float width;
    float height;
    float gravity;
    float powerJump;
    bool inSky;
    bool requestJump;
    bool leftLock;
    bool rightLock;
} EntityPlatformer;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} EntityTopdown;

EntityPlatformer *hitbox_platforme(EntityPlatformer *entite, Blocks *blocs, float vitesse_max_chute,
                                   float correction_mur, int *types_ignorer, int nb_ignorer);
EntityTopdown *hitbox_topdown(EntityTopdown *entity, Blocks *blocks, int *types_ignorer,
                              int nb_ignorer);

#ifdef __cplusplus
}
#endif

#endif /* HITBOX_H */