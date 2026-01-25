/*
 * Ajout d'objets et de primitives géométriques au rendu.
 */

#ifndef AJOUT_H
#define AJOUT_H

#include "../../../proprietes.h"
#include <SDL.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Liste Liste;

typedef enum { TYPE_IMAGE = 0, TYPE_FORME = 1 } TypeObjet;

typedef struct {
    Uint8 r, g, b, a;
    float posx, posy;
    float posx2, posy2;
    Sint16 taillex, tailley;
    bool plein;
    int type_de_forme;
} Forme;

typedef struct {
    float posx, posy;
    Sint16 taillex, tailley;
    bool sens;
    Uint16 rotation;
    Uint8 a;
    SDL_Texture *texture;
} Image;

typedef struct {
    TypeObjet type;
    Image image;
    Forme forme;
} ObjectImage;

void ajouter_image_au_jeu(ObjectImage nouvelle);

void ajouter_image_au_tableau(const char *id, float x, float y, Sint16 taillex, Sint16 tailley,
                              bool sens, Uint16 rotation, Uint8 a);

void ajouter_ligne(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b);

void ajouter_rectangle(float x, float y, Sint16 taillex, Sint16 tailley, Uint8 r, Uint8 g, Uint8 b,
                       Uint8 a, bool plein);

void ajouter_cercle(float x, float y, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, bool plein);

void ajouter_triangle(float x, float y, Sint16 taillex, Sint16 tailley, Uint8 r, Uint8 g, Uint8 b,
                      bool plein);

#ifdef __cplusplus
}
#endif

#endif /* AJOUT_H */