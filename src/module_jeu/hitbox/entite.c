/* gere la creation et la suppresion dentites et le tableau dentites */

#include "../../allouer/allouer.h"
#include "../../prediction_branche.h"
#include "hitbox.h"

/* creer une entite */
EntitePlatforme *creer_entite_platforme(float x, float y, float w, float h, float force_saut,
                                        float gravite, float vitesse_x_max, float vitesse_max_chute,
                                        float correction_mur, float vitesse_initiale,
                                        float acceleration, int nb_sauts, bool saut_sur_murs) {

    EntitePlatforme *res = malloc_gestion_echec_compteur(sizeof(EntitePlatforme));
    res->height = h;
    res->width = w;
    res->x = x;
    res->y = y;
    res->powerJump = force_saut;
    res->gravity = gravite;
    res->leftLock = false;
    res->rightLock = false;
    res->speedY = 0.0f;
    res->inSky = false;
    res->requestJump = false;
    res->requestLeft = false;
    res->requestRight = false;
    res->speedX = 0.0f;
    res->speedMaxX = vitesse_x_max;
    res->speedMaxFall = vitesse_max_chute;
    res->wallCorrection = correction_mur;
    res->initialSpeed = vitesse_initiale;
    res->acceleration = acceleration;
    res->numberOfJumps = nb_sauts;
    res->numberOfJumpsPossible = nb_sauts;
    res->jumpOnWall = saut_sur_murs;
    res->wallTimerLeft = 0.0f;
    res->wallTimerRight = 0.0f;
    return res;
}

/* libere une entite */
void liberer_entite_platforme(EntitePlatforme *entite) { free_gestion_echec_compteur(entite); }

/* creer un tableau dentites */
EntitePlatformes *creer_tableau_entite(void) {
    EntitePlatformes *ptr = malloc_gestion_echec_compteur(sizeof(EntitePlatformes));
    ptr->capacite = 10;
    ptr->taille = 0;
    ptr->entites = malloc_gestion_echec_compteur(sizeof(EntitePlatforme) * ptr->capacite);
    return ptr;
}

/* liberer un tableau dentites */
void liberer_entites_platforme(EntitePlatformes *entites) {
    if (UNLIKELY(!entites))
        return;

    free_gestion_echec_compteur(entites->entites);
    free_gestion_echec_compteur(entites);
}

/* ajoute une entite au tableau dentites */
void ajouter_entite_platforme(EntitePlatformes *entites, EntitePlatforme *entite) {
    if (UNLIKELY(!entites || !entite))
        return;

    if (entites->taille + 1 >= entites->capacite) {
        entites->capacite *= 2;
        entites->entites = realloc_gestion_echec_compteur(
            entites->entites, sizeof(EntitePlatforme) * entites->capacite);
    }
    entites->entites[entites->taille] = *entite;
    entites->taille++;
}