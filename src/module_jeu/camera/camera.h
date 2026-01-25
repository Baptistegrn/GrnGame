#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x, y;
    float smooth_factor; // facteur de lissage
    int width, height;   // taille camera
} Camera;

void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt);

#endif /* CAMERA_H */