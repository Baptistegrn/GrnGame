/*
 * Gestionnaire d'allocation memoire securise.
 * Fournit des wrappers autour de malloc/realloc/free avec gestion des erreurs.
 */

#ifndef ALLOUER_H
#define ALLOUER_H

#include <stddef.h>

/* Allocation memoire securisee avec comptage */
void *malloc_gestion_echec_compteur(size_t taille);

/* Reallocation memoire securisee avec comptage */
void *realloc_gestion_echec_compteur(void *bloc, size_t taille);

/* Liberation memoire avec decrementation du compteur */
void free_gestion_echec_compteur(void *ptr);

#endif /* ALLOUER_H */