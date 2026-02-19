/*
 * Mise a jour de la camera avec interpolation.
 */
#include "camera.h"
#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../../moteur/logging/logging.h"
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
    gs->camera->x = x;
    gs->camera->y = y;
    gs->camera->smooth_factor = smooth_facteur;
    gs->camera->width = w;
    gs->camera->height = h;
    return;
gsvide:
    log_message(NiveauLogErreur, "manager empty in creation of camera");
}

/* initialise une camera dans le gs */
void initialiser_camera(void) {
    gs->camera = malloc_gestion_echec_compteur(sizeof(Camera));
    memset(gs->camera, 0, sizeof(Camera));
}