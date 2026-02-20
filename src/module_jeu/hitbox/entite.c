/* gere la creation et la suppresion dentites */

#include "../../allouer/allouer.h"
#include "hitbox.h"

/* creer une entite */
EntityPlatformer *creer_entite_platforme(float x, float y, float w, float h, float force_saut,
                                         float gravite, float vitesse_x_max,
                                         float vitesse_max_chute, float correction_mur,
                                         float vitesse_initiale, float acceleration) {

    EntityPlatformer *res = malloc_gestion_echec_compteur(sizeof(EntityPlatformer));
    res->height = h;
    res->width = w;
    res->x = x;
    res->y = y;
    res->powerJump = force_saut;
    res->gravity = gravite;
    res->leftLock = false;
    res->rightLock = false;
    res->speedY = 0;
    res->inSky = false;
    res->requestJump = false;
    res->requestLeft = false;
    res->requestRight = false;
    res->speedX = 0;
    res->speedMaxX = vitesse_x_max;
    res->speedMaxFall = vitesse_max_chute;
    res->wallCorrection = correction_mur;
    res->initialSpeed = vitesse_initiale;
    res->acceleration = acceleration;
    return res;
}

/* libere une entite */
void liberer_entite_platforme(EntityPlatformer *entite) { free_gestion_echec_compteur(entite); }
