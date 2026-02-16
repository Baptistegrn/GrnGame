/*
 * Gestion des conteneurs de blocs de collision.
 */

#include "bloc.h"
#include "../../allouer/allouer.h"

/* Cree un conteneur de blocs vide avec capacite initiale de 10 */
Blocks *creer_blocks(void) {
    Blocks *ptr = malloc_gestion_echec_compteur(sizeof(Blocks));
    ptr->size = 0;
    ptr->capacity = 10;
    ptr->tab = malloc_gestion_echec_compteur(sizeof(Block) * ptr->capacity);
    return ptr;
}

/* Ajoute un bloc au conteneur, agrandit si necessaire */
void ajouter_block(Blocks *blocks, Block *block) {
    if (blocks->size >= blocks->capacity) {
        blocks->capacity *= 2;
        blocks->tab = realloc_gestion_echec_compteur(blocks->tab, sizeof(Block) * blocks->capacity);
    }
    blocks->tab[blocks->size].x = block->x;
    blocks->tab[blocks->size].y = block->y;
    blocks->tab[blocks->size].w = block->w;
    blocks->tab[blocks->size].h = block->h;
    blocks->tab[blocks->size].type = block->type;
    blocks->size++;
}

/* Retourne le nombre de blocs dans le conteneur */
int taille_blocks(Blocks *blocks) {
    if (blocks) {
        return blocks->size;
    }
    return 0;
}

/* creer un block */
Block *creer_block(float x, float y, float w, float h, int type) {
    Block *ptr = malloc_gestion_echec_compteur(sizeof(Block));
    ptr->x = x;
    ptr->y = y;
    ptr->w = w;
    ptr->h = h;
    ptr->type = type;
    return ptr;
}