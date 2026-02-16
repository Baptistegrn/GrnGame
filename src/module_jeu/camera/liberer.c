/*
 * Liberation de la camera.
 */

#include "../../allouer/allouer.h"
#include "camera.h"

/* libere une camera */
void liberer_camera(Camera *ptr) {
    if (!ptr)
        return;

    free_gestion_echec_compteur(ptr);
}