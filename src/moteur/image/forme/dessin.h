/*
 * Primitives geometriques de dessin (ligne, cercle, rectangle, triangle).
 */

#ifndef DESSIN_H
#define DESSIN_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

void dessiner_ligne_pixel(float x0, float y0, float x1, float y1, Uint8 r, Uint8 g, Uint8 b,
                          Uint8 a);

void dessiner_cercle_vide(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void dessiner_cercle_plein(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void dessiner_rectangle_vide(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                             Uint8 a);

void dessiner_rectangle_plein(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                              Uint8 a);

void dessiner_points(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void dessiner_points_n(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                       float n);

void tracer_ligne_horizontale_float(float x, float y, float longueur, Uint8 r, Uint8 g, Uint8 b,
                                    Uint8 a);
void tracer_ligne_verticale_float(float x, float y, float longueur, Uint8 r, Uint8 g, Uint8 b,
                                  Uint8 a);
#ifdef __cplusplus
}
#endif

#endif