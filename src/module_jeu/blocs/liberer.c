/*
 * Liberation des conteneurs de blocs.
 */

#include "../../main.h"

/* Libere un conteneur de blocs et son tableau */
void liberer_blocks(Blocks *ptr) {
    if (!ptr)
        return;

    free_gestion_echec_compteur(ptr->tab);
    ptr->tab = NULL;
    free_gestion_echec_compteur(ptr);
}
