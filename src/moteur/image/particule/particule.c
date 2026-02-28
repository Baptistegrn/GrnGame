/* creation de la structure de particules et du tableau de particules */

#include "particule.h"
#include "../../../allouer/allouer.h"
#include <stddef.h>
#include <stdint.h>

/* creer une seule particule */
Particule *creer_particule(float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint16 rotation) {
    Particule *ptr = malloc_gestion_echec_compteur(sizeof(Particule));
    ptr->rotation = rotation;
    ptr->x = x;
    ptr->y = y;
    ptr->r = r;
    ptr->g = g;
    ptr->b = b;
    ptr->a = a;
    return ptr;
}

/*creer un tableau de particules */
Particules *creer_particules(void) {
    Particules *ptr = malloc_gestion_echec_compteur(sizeof(Particule));
    ptr->taille = 0;
    ptr->capacite = 200;
    ptr->tab = malloc_gestion_echec_compteur(sizeof(Particule) * ptr->capacite);
    return ptr;
}

/*ajoute une particule au tableau de particules */
void ajouter_particule(Particules *tab, Particule *p) {
    if (tab->taille >= tab->capacite) {
        tab->capacite = tab->capacite * 2;
        tab->tab = realloc_gestion_echec_compteur(tab->tab, sizeof(Particule) * tab->capacite);
    }
    tab->tab[tab->taille] = *p;
    tab->taille++;
}
