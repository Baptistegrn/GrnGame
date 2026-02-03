/*
 * Gestionnaire d'allocation memoire securise.
 * Termine le programme en cas d'echec d'allocation.
 * En mode MALLOC_MODE, affiche le compteur d'allocations.
 */

#include "../main.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

/* Compteur d'allocations actives */
static Uint32 compteur = 0;

/*
 * Alloue de la memoire avec gestion d'erreur.
 * Quitte le programme si l'allocation echoue.
 */
void *malloc_gestion_echec_compteur(size_t taille) {
    void *ptr = malloc(taille);

    if (!ptr && taille > 0) {
        printf("allocation failure\n");
        exit(EXIT_FAILURE);
    }

    compteur += 1;
#ifdef MALLOC_MODE
    log_fmt(NiveauLogDebug, "+malloc : %" PRIu32 "\n", compteur);
#endif
    return ptr;
}

/*
 * Realloue de la memoire avec gestion d'erreur.
 * Quitte le programme si la reallocation echoue.
 */
void *realloc_gestion_echec_compteur(void *bloc, size_t taille) {
    if (!bloc) {
        compteur++;
    }
    void *ptr = realloc(bloc, taille);
    if (!ptr && taille > 0) {
        printf("allocation failure\n");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

/* Libere la memoire et decremente le compteur */
void free_gestion_echec_compteur(void *ptr) {
    if (!ptr) {
        return;
    }
    free(ptr);
    compteur--;
#ifdef MALLOC_MODE
    log_fmt(NiveauLogDebug, "+free : %" PRIu32 "\n", compteur);
#endif
}
