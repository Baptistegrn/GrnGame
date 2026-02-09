/*
 * Systeme de camera avec suivi fluide.
 */

#ifndef CAMERA_H
#define CAMERA_H

/* Structure de camera avec position et lissage */
typedef struct {
    float x, y;          /* Position de la camera */
    float smooth_factor; /* Facteur de lissage du mouvement */
    int width, height;   /* Dimensions de la vue */
} Camera;

/* Met a jour la position de la camera vers une cible */
void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt);

/* creer une camera */
Camera *creer_camera(float x, float y, float smooth_facteur, int w, int h);

/* libere une camera */
void liberer_camera(Camera *ptr);

#endif /* CAMERA_H */