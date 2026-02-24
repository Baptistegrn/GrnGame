/*
 * Mise a jour de la camera avec interpolation.
 */
#include "camera.h"
#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../../moteur/logging/logging.h"
#include "../module_jeu.h"
#include <math.h>
#include <string.h>

/*
 * Deplace la camera vers une cible avec interpolation lineaire.
 * Le facteur de lissage controle la vitesse de suivi.
 */
void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt) {
    if (!cam)
        return;

    float cible_cam_x = cible_x - cam->width * 0.5f;
    float cible_cam_y = cible_y - cam->height * 0.5f;

    float dx = cible_cam_x - cam->x;
    float dy = cible_cam_y - cam->y;
    cam->x += dx * cam->smooth_factor * dt;
    cam->y += dy * cam->smooth_factor * dt;
}

/* creer une camera */
void creer_camera(float x, float y, float smooth_facteur, int w, int h) {
    if (!gs)
        goto gsvide;
    gs->module_jeu->camera->x = x;
    gs->module_jeu->camera->y = y;
    gs->module_jeu->camera->smooth_factor = smooth_facteur;
    gs->module_jeu->camera->width = w;
    gs->module_jeu->camera->height = h;
    return;
gsvide:
    log_message(NiveauLogErreur, "manager empty in creation of camera");
}

/* initialise une camera dans le gs */
void initialiser_camera(void) {
    gs->module_jeu->camera = malloc_gestion_echec_compteur(sizeof(Camera));
    memset(gs->module_jeu->camera, 0, sizeof(Camera));
}