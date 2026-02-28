/*
 * Gestion des conteneurs de blocs de collision.
 */

#include "bloc.h"
#include "../../allouer/allouer.h"
#include "../../moteur/logging/logging.h"

/* Cree un conteneur de blocs vide avec capacite initiale de 10 */
Blocs *creer_blocks(void) {
    Blocs *ptr = malloc_gestion_echec_compteur(sizeof(Blocs));
    ptr->size = 0;
    ptr->capacity = 10;
    ptr->tab = malloc_gestion_echec_compteur(sizeof(Bloc) * ptr->capacity);
    return ptr;
}

/* Ajoute un bloc au conteneur, agrandit si necessaire */
void ajouter_block(Blocs *blocks, Bloc *block) {
    if (blocks->size >= blocks->capacity) {
        blocks->capacity *= 2;
        blocks->tab = realloc_gestion_echec_compteur(blocks->tab, sizeof(Bloc) * blocks->capacity);
    }
    blocks->tab[blocks->size].x = block->x;
    blocks->tab[blocks->size].y = block->y;
    blocks->tab[blocks->size].w = block->w;
    blocks->tab[blocks->size].h = block->h;
    blocks->tab[blocks->size].type = block->type;
    blocks->size++;
}

/* Retourne le nombre de blocs dans le conteneur */
int taille_blocks(Blocs *blocks) {
    if (blocks) {
        return blocks->size;
    }
    return 0;
}

/* creer un block */
Bloc *creer_block(float x, float y, float w, float h, int type) {
    Bloc *ptr = malloc_gestion_echec_compteur(sizeof(Bloc));
    ptr->x = x;
    ptr->y = y;
    ptr->w = w;
    ptr->h = h;
    ptr->type = type;
    return ptr;
}