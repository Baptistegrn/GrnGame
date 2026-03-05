/*
 * Gestionnaire d'allocation memoire securise.
 * Termine le programme en cas d'echec d'allocation.
 */

#include "allouer.h"
#include "../headers_sdl/SDL_stdinc.h"
#include "../moteur/logging/logging.h"
#include "../proprietes.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/* Compteur d'allocations actives */
#ifdef DEBUG_MODE
static Uint32 compteur = 0;
#endif

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

#ifdef DEBUG_MODE
    compteur += 1;
    if (compteur % AFFICHAGE_COMPTEUR == 0)
        log_fmt(NiveauLogDebug, "+ allocation : %" PRIu32 "\n", compteur);
#endif

    return ptr;
}

/*
 * Realloue de la memoire avec gestion d'erreur.
 * Quitte le programme si la reallocation echoue.
 */
void *realloc_gestion_echec_compteur(void *bloc, size_t taille) {
#ifdef DEBUG_MODE
    if (!bloc) {
        compteur++;
    }
#endif

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

#ifdef DEBUG_MODE
    compteur--;
    if (compteur % AFFICHAGE_COMPTEUR == 0)
        log_fmt(NiveauLogDebug, "+ free : %" PRIu32 "\n", compteur);
#endif
}
