#include "../../main.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ajouter_ligne(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                   int g, int b) {
  if (!gs)
    return;

  Object_image obj;
  memset(&obj, 0, sizeof(Object_image));

  obj.type = TYPE_FORME;
  obj.forme.type_de_forme = 1; // 1 = Ligne
  obj.forme.posx = x1;
  obj.forme.posy = y1;
  obj.forme.posx2 = x2;
  obj.forme.posy2 = y2;
  obj.forme.r = r;
  obj.forme.g = g;
  obj.forme.b = b;
  obj.forme.plein = 1;

  ajouter_image_au_jeu(gs, obj);
}

void ajouter_rectangle(Gestionnaire *gs, int x, int y, int w, int h, int r,
                       int g, int b, int plein) {
  if (!gs)
    return;

  Object_image obj;
  memset(&obj, 0, sizeof(Object_image));

  obj.type = TYPE_FORME;
  obj.forme.type_de_forme = 2; // 2 = Rectangle
  obj.forme.posx = x;
  obj.forme.posy = y;
  obj.forme.posx2 = w;
  obj.forme.posy2 = h;
  obj.forme.r = r;
  obj.forme.g = g;
  obj.forme.b = b;
  obj.forme.plein = plein;

  ajouter_image_au_jeu(gs, obj);
}

void ajouter_cercle(Gestionnaire *gs, int x, int y, int rayon, int r, int g,
                    int b, int plein) {
  if (!gs)
    return;

  Object_image obj;
  memset(&obj, 0, sizeof(Object_image));

  obj.type = TYPE_FORME;
  obj.forme.type_de_forme = 3; // 3 = Cercle
  obj.forme.posx = x;
  obj.forme.posy = y;
  obj.forme.posx2 = rayon * 2; // Diametre
  obj.forme.posy2 = rayon * 2;
  obj.forme.r = r;
  obj.forme.g = g;
  obj.forme.b = b;
  obj.forme.plein = plein;

  ajouter_image_au_jeu(gs, obj);
}

void ajouter_triangle(Gestionnaire *gs, int x, int y, int w, int h, int r,
                      int g, int b, int plein) {
  if (!gs)
    return;

  Object_image obj;
  memset(&obj, 0, sizeof(Object_image));

  obj.type = TYPE_FORME;
  obj.forme.type_de_forme = 4; // 4 = rectangle
  obj.forme.posx = x;
  obj.forme.posy = y;
  obj.forme.posx2 = w;
  obj.forme.posy2 = h;
  obj.forme.r = r;
  obj.forme.g = g;
  obj.forme.b = b;
  obj.forme.plein = plein;

  ajouter_image_au_jeu(gs, obj);
}