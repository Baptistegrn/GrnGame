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
    float gravity;             /* Force de gravite */
    float speedMaxX;           /* Vitesse entite horizontale max */
    float powerJump;           /* Force de saut */
    bool inSky;                /* En l'air */
    bool requestJump;          /* Demande de saut */
    bool requestLeft;          /* Demande de se deplacer a gauche */
    bool requestRight;         /* Demande de se deplacer a droite */
    bool leftLock;             /* Bloque a gauche */
    bool rightLock;            /* Bloque a droite */
    float wallCorrection;      /* freine si coller a un mur */
    float speedMaxFall;        /* vitesse max de chute */
    float acceleration;        /* acceleration */
    float initialSpeed;        /* vitesse initiale x*/
    int numberOfJumps;         /* nombres de saut actuel */
    int numberOfJumpsPossible; /* nombres de saut possible max */
    bool jumpOnWall;           /* saute sur les murs */
    float wallTimerLeft;       /* timer tolerence saut mur */
    float wallTimerRight;      /* timer tolerence saut mur */
    float groundTimer;         /* timer tolerence saut sur le sol */
    float wallJumpLockTimer;   /* bloquer la gauche ou la droite selon le saut sur le mur */

} EntitePlatforme;

/* tableau dentites */
typedef struct {
    int taille;
    int capacite;
    EntitePlatforme *entites;
} EntitePlatformes;

/* gere la hitblox selon une liste de blocs et dentites */
void hitbox_platforme(EntitePlatformes *entites, Blocs *blocs, Uint32 masque_ignorer, float dt);

/* declenche le calcul de la hitbox */
void lancer_calcul_hitbox(EntitePlatformes *entites, Blocs *blocs, Uint32 masque, float dt);

/* Cree une entite de plateforme */
EntitePlatforme *creer_entite_platforme(float x, float y, float w, float h, float force_saut,
                                        float gravite, float vitesse_x_max, float vitesse_max_chute,
                                        float correction_mur, float vitesse_initiale,
                                        float acceleration, int nb_sauts, bool saut_sur_murs);

/* Libere une entite de plateforme */
void liberer_entite_platforme(EntitePlatforme *entite);

/* ajoute une entite au tableau dentites */
void ajouter_entite_platforme(EntitePlatformes *entites, EntitePlatforme *entite);

/* liberer un tableau dentites */
void liberer_entites_platforme(EntitePlatformes *entites);

/* creer un tableau dentites */
EntitePlatformes *creer_tableau_entite(void);

#ifdef __cplusplus
}
#endif

#endif /* HITBOX_H */