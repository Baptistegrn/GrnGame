/* creation de la structure de particules et du tableau de particules */

#ifndef PARTICULE_H
#define PARTICULE_H

#include <SDL.h>

typedef struct {
    float x;
    float y;          /* positions */
    Uint16 rotation;  /* rotation */
    Uint8 a, r, g, b; /* couleurs */
} Particule;

typedef struct {
    int taille;     /* taille de particules */
    int capacite;   /* capacite de particules */
    Particule *tab; /* tableau de particules */
} Particules;

/*ajoute une particule au tableau de particules */
void ajouter_particule(Particules *tab, Particule *p);

/* creer une seule particule */
Particule *creer_particule(float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint16 rotation);

/*creer un tableau de particules */
Particules *creer_particules(void);

/* libere une particule */
void liberer_particule(Particule *ptr);

/* libere un tableau de particules */
void liberer_particules(Particules *ptr);

#endif /* PARTICULE_H */