/*
 * Mise a jour de la camera avec interpolation.
 */

#include "../../main.h"

/*
 * Deplace la camera vers une cible avec interpolation lineaire.
 * Le facteur de lissage controle la vitesse de suivi.
 */
void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt) {
    if (!cam)
        return;

    float cible_cam_x = cible_x - cam->width / cam->smooth_factor;
    float cible_cam_y = cible_y - cam->height / cam->smooth_factor;

    cam->x += (cible_cam_x - cam->x) * cam->smooth_factor * dt;
    cam->y += (cible_cam_y - cam->y) * cam->smooth_factor * dt;
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