/*
 * Structures et fonctions pour la gestion des blocs de collision.
 */

#ifndef BLOC_H
#define BLOC_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Un bloc avec position, taille et type */
typedef struct {
    float x;
    float y;
    float w;
    float h;
    int type;
} Bloc;

/* Conteneur dynamique de blocs */
typedef struct {
    Bloc *tab;    /* Tableau de blocs */
    int size;     /* Nombre de blocs */
    int capacity; /* Capacite du tableau */
} Blocs;

/* Cree un conteneur de blocs vide */
Blocs *creer_blocks(void);

/* Ajoute un bloc au conteneur */
void ajouter_block(Blocs *blocks, Bloc *block);

/* Retourne le nombre de blocs */
int taille_blocks(Blocs *blocks);

/* Libere un conteneur de blocs */
void liberer_blocks(Blocs *ptr);

/* creer un block */
Bloc *creer_block(float x, float y, float w, float h, int type);

/* Libere un bloc */
void liberer_block(Bloc *ptr);

#ifdef __cplusplus
}
#endif

#endif /* BLOC_H */