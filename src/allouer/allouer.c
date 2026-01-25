#include "../main.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

/*
 * Allocation mémoire sécurisée avec verrouilage en cas d'échec.
 * Quitte le programme si la mémoire système est insuffisante.
 */

static Uint32 compteur = 0;

void *xmalloc(size_t taille) {
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
 * Réallocation mémoire sécurisée avec verrouilage en cas d'échec.
 * Quitte le programme si la mémoire système est insuffisante.
 */
void *xrealloc(void *bloc, size_t taille) {
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

void xfree(void *ptr) {
    if (!ptr) {
        return;
    }
    free(ptr);
    compteur--;
#ifdef MALLOC_MODE
    log_fmt(NiveauLogDebug, "+free : %" PRIu32 "\n", compteur);
#endif
}
