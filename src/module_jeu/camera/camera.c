#include "../../main.h"

/* met a jour le systeme de camera selon dt et un facteur lissant */
void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt) {
    if (!cam)
        return;

    float cible_cam_x = cible_x - cam->width / cam->smooth_factor;
    float cible_cam_y = cible_y - cam->height / cam->smooth_factor;

    cam->x += (cible_cam_x - cam->x) * cam->smooth_factor * dt;
    cam->y += (cible_cam_y - cam->y) * cam->smooth_factor * dt;
}