/*
 * Mise a jour de la camera avec interpolation.
 */

#include "camera.h"
#include "../../allouer/allouer.h"
#include <math.h>
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
Camera *creer_camera(float x, float y, float smooth_facteur, int w, int h) {
    Camera *cam = malloc_gestion_echec_compteur(sizeof(Camera));
    cam->x = x;
    cam->y = y;
    cam->smooth_factor = smooth_facteur;
    cam->width = w;
    cam->height = h;
    return cam;
}