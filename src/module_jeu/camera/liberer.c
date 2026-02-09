/*
 * Liberation de la camera.
 */

#include "../../main.h"

/* libere une camera */
void liberer_camera(Camera *ptr) {
    if (!ptr)
        return;

    free_gestion_echec_compteur(ptr);
}