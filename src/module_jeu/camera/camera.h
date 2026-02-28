/*
 * Systeme de camera avec suivi fluide.
 */

#ifndef CAMERA_H
#define CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Structure de camera avec position et lissage */
typedef struct Camera {
    float x, y;            /* Position de la camera */
    float facteur_lissage; /* Facteur de lissage du mouvement */
    int w, h;              /* Dimensions de la vue */
    float limite_bas, limite_haut, limite_gauche, limite_droite; /* limite camera */

    /* tremblement */
    float tremblement_intensite; /* Intensite courante du tremblement */
    float tremblement_duree;     /* Duree restante du tremblement (secondes) */
    float tremblement_decay;     /* Facteur de reduction par seconde */
} Camera;

/* Met a jour la position de la camera vers une cible */
void camera_mise_a_jour(Camera *cam, float cible_x, float cible_y, float dt);

/* Declenche un tremblement de camera */
void camera_trembler(Camera *cam, float intensite, float duree, float decay);

/* creer une camera */
void creer_camera(float x, float y, float smooth_facteur, int w, int h, float limite_gauche,
                  float limite_droite, float limite_haut, float limite_bas);

/* libere une camera */
void liberer_camera(Camera *ptr);

/* initialise une camera dans le gs */
void initialiser_camera(void);

Camera *retourner_camera();

#ifdef __cplusplus
}
#endif

#endif /* CAMERA_H */