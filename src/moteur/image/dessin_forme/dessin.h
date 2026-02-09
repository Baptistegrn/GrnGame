/*
 * Primitives geometriques de dessin (ligne, cercle, rectangle, triangle).
 */

#ifndef DESSIN_H
#define DESSIN_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dessine une ligne pixel par pixel entre deux points */
void dessiner_ligne_pixel(float x0, float y0, float x1, float y1, Uint8 r, Uint8 g, Uint8 b,
                          Uint8 a);

/* Dessine un cercle vide */
void dessiner_cercle_vide(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Dessine un cercle plein */
void dessiner_cercle_plein(float xc, float yc, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Dessine un rectangle vide */
void dessiner_rectangle_vide(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                             Uint8 a);

/* Dessine un rectangle plein */
void dessiner_rectangle_plein(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b,
                              Uint8 a);

/* Dessine deux points aux positions donnees */
void dessiner_points(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Dessine deux points avec epaisseur n */
void dessiner_points_n(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                       float n);

/* Trace une ligne horizontale avec coordonnees float */
void tracer_ligne_horizontale_float(float x, float y, float longueur, Uint8 r, Uint8 g, Uint8 b,
                                    Uint8 a);

/* Trace une ligne verticale avec coordonnees float */
void tracer_ligne_verticale_float(float x, float y, float longueur, Uint8 r, Uint8 g, Uint8 b,
                                  Uint8 a);
#ifdef __cplusplus
}
#endif

#endif /* DESSIN_H */