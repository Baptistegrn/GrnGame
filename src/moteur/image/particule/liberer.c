/* libere les structures de particules */

#include "../../../allouer/allouer.h"
#include "../../../prediction_branche.h"
#include "particule.h"
#include <stdint.h>

/* libere un tableau de particules */
void liberer_particules(Particules *ptr) {
    if (LIKELY(ptr)) {
        free_gestion_echec_compteur(ptr);
        free_gestion_echec_compteur(ptr->tab);
    }
}

/* libere une particule */
void liberer_particule(Particule *ptr) {
    if (LIKELY(ptr)) {
        free_gestion_echec_compteur(ptr);
    }
}