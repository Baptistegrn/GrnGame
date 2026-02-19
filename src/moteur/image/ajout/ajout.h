/*
 * Ajout d'objets et de primitives geometriques au rendu.
 */

#ifndef AJOUT_H
#define AJOUT_H

#include "../../../proprietes.h"
#include "SDL_stdinc.h"
#include <SDL.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Liste Liste;

typedef enum { TYPE_IMAGE = 0, TYPE_FORME = 1, TYPE_PARTICULE = 2 } TypeObjet;

typedef struct {
    Uint8 r, g, b, a;        /* couleur */
    float posx, posy;        /* position */
    float posx2, posy2;      /* position 2 (pour ligne) */
    Sint16 taillex, tailley; /* taille */
    bool plein;              /* plein ou vide */
    int type_de_forme;       /* 1=ligne,2=rectangle,3=cercle */

} Forme;

typedef struct {
    float posx, posy;        /* position */
    Sint16 taillex, tailley; /* taille */
    bool sens;               /* flip horizontal */
    Uint16 rotation_p;       /* angle de rotation pixel perfect */
    Uint16 rotation;         /* angle de rotation non pixel perfect */
    Uint8 a;                 /* alpha */
    SDL_Texture *texture;
    bool sprite; /* est un sprite */
    /* sprite position */
    Sint16 x1, y1;
    Sint16 x2, y2;
} Image;

typedef struct {
    float *posx, *posy;   /* positions */
    Uint16 *rotation;     /* rotations */
    Uint8 *a, *r, *g, *b; /* couleurs */
    int taille;           /* nombre de particules */
} Particule;

typedef struct {
    TypeObjet type;
    Image image;         /* images */
    Forme forme;         /* formes geometriques */
    Particule particule; /* particules */
} ObjectImage;

typedef struct {
    const char id[TAILLE_LIEN]; /* lien */
    Sint16 taillex, tailley;    /* taille */
} Sprite;

/* reallocation automatique du tableau dimage */
void reallouer_si_plein(void);

/* Ajoute une image au tableau par son chemin */
void ajouter_image_au_tableau(const char *id, float x, float y, Sint16 coeff, bool sens,
                              Uint16 rotation_p, Uint16 rotation, Uint8 a);

/* Ajoute une ligne au rendu */
void ajouter_ligne(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Ajoute un rectangle au rendu */
void ajouter_rectangle(float x, float y, Sint16 taillex, Sint16 tailley, Uint8 r, Uint8 g, Uint8 b,
                       Uint8 a, bool plein);

/* Ajoute un cercle au rendu */
void ajouter_cercle(float x, float y, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool plein);

/* Cree un sprite avec une taille de frame */
Sprite *creer_sprite(const char *id, Sint16 taillex, Sint16 tailley);

/* Ajoute un sprite au tableau par index de frame */
void ajouter_sprite_au_tableau(Sprite *sprite, Sint16 index, float x, float y, Sint16 coeff,
                               bool sens, Uint16 rotation, Uint8 a);

/* Ajoute un groupe de particules au tableau */
void ajouter_particules_au_tableau(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r,
                                   Uint8 *g, Uint8 *b, int taille);

/* Libere un sprite */
void liberer_sprite(Sprite *sprite);
#ifdef __cplusplus
}
#endif

#endif /* AJOUT_H */