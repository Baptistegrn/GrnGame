/*
 * Mise a jour de la camera avec interpolation.
 */
#include "camera.h"
#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../../moteur/logging/logging.h"
#include "../module_jeu.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * Deplace la camera vers une cible avec interpolation lineaire.
 * Le facteur de lissage controle la vitesse de suivi.
 */
void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt) {
    if (!cam)
        return;

    float cible_cam_x = cible_x - cam->w * 0.5f;
    float cible_cam_y = cible_y - cam->h * 0.5f;

    float dx = cible_cam_x - cam->x;
    float dy = cible_cam_y - cam->y;
    cam->x += dx * cam->facteur_lissage * dt;
    cam->y += dy * cam->facteur_lissage * dt;

    /* limites */
    if (cam->x < cam->limite_gauche)
        cam->x = cam->limite_gauche;
    else if (cam->x + cam->w > cam->limite_droite)
        cam->x = cam->limite_droite - cam->w;

    if (cam->y < cam->limite_haut)
        cam->y = cam->limite_haut;
    else if (cam->y + cam->h > cam->limite_bas)
        cam->y = cam->limite_bas - cam->h;

    /* tremblement */
    if (cam->tremblement_duree > 0.0f) {
        float offset_x = ((float)(rand() % 2001) / 1000.0f - 1.0f) * cam->tremblement_intensite;
        float offset_y = ((float)(rand() % 2001) / 1000.0f - 1.0f) * cam->tremblement_intensite;
        cam->x += offset_x;
        cam->y += offset_y;

        cam->tremblement_intensite -= cam->tremblement_decay * dt;
        if (cam->tremblement_intensite < 0.0f)
            cam->tremblement_intensite = 0.0f;

        cam->tremblement_duree -= dt;
        if (cam->tremblement_duree < 0.0f)
            cam->tremblement_duree = 0.0f;
    }
}

/*
 * Declenche un tremblement de camera
 */
void camera_trembler(Camera *cam, float intensite, float duree, float decay) {
    if (!cam)
        return;
    cam->tremblement_intensite = intensite;
    cam->tremblement_duree = duree;
    cam->tremblement_decay = decay;
}

/* creer une camera */
void creer_camera(float x, float y, float facteur_l, int w, int h, float l1, float l2, float l3,
                  float l4) {
    if (!gs)
        goto gsvide;
    gs->module_jeu->camera->x = x;
    gs->module_jeu->camera->y = y;
    gs->module_jeu->camera->facteur_lissage = facteur_l;
    gs->module_jeu->camera->w = w;
    gs->module_jeu->camera->h = h;
    gs->module_jeu->camera->limite_gauche = l1;
    gs->module_jeu->camera->limite_droite = l2;
    gs->module_jeu->camera->limite_haut = l3;
    gs->module_jeu->camera->limite_bas = l4;
    return;
gsvide:
    log_message(NiveauLogErreur, "manager empty in creation of camera");
}

/* initialise une camera dans le gs */
void initialiser_camera(void) {
    gs->module_jeu->camera = malloc_gestion_echec_compteur(sizeof(Camera));
    memset(gs->module_jeu->camera, 0, sizeof(Camera));
}

/* retourne la camera */
Camera *retourner_camera() {
    if (gs)
        return gs->module_jeu->camera;

    return NULL;
}