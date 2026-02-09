/*
 * Structures et fonctions pour la gestion des blocs de collision.
 */

#ifndef BLOC_H
#define BLOC_H

/* Un bloc avec position, taille et type */
typedef struct {
    float x;
    float y;
    float w;
    float h;
    int type;
} Block;

/* Conteneur dynamique de blocs */
typedef struct {
    Block *tab;   /* Tableau de blocs */
    int size;     /* Nombre de blocs */
    int capacity; /* Capacite du tableau */
} Blocks;

/* Cree un conteneur de blocs vide */
Blocks *creer_blocks(void);

/* Ajoute un bloc au conteneur */
void ajouter_block(Blocks *blocks, Block *block);

/* Retourne le nombre de blocs */
int taille_blocks(Blocks *blocks);

/* Libere un conteneur de blocs */
void liberer_blocks(Blocks *ptr);

/* creer un block */
Block *creer_block(float x, float y, float w, float h, int type);

/* Libere un bloc */
void liberer_block(Block *ptr);

#endif /* BLOC_H */