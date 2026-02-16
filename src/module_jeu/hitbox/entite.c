/* creer les entite platformer et topdown et les liberent*/

#include "../../allouer/allouer.h"
#include "hitbox.h"

EntityPlatformer *creer_entite_platforme(float x, float y, float w, float h, float force_saut,
                                         float gravite) {

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
    return res;
}

EntityTopdown *creer_entite_topdown(float x, float y, float w, float h) {
    EntityTopdown *res = malloc_gestion_echec_compteur(sizeof(EntityTopdown));
    res->height = h;
    res->width = w;
    res->x = x;
    res->y = y;
    return res;
}

void liberer_entite_platforme(EntityPlatformer *entite) { free_gestion_echec_compteur(entite); }

void liberer_entite_topdown(EntityTopdown *entite) { free_gestion_echec_compteur(entite); }