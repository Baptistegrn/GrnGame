#include "../../logging/logging.h"
#include "../../main.h"
#include <math.h>


bool hors_ecran(Gestionnaire *gs, int posx, int posy, int taillex, int tailley,
                int decalage_x_scaled, int decalage_y_scaled) {
  if (posx - decalage_x_scaled > gs->largeur ||
      posx + taillex + decalage_x_scaled < 0 ||
      posy - decalage_y_scaled > gs->hauteur ||
      posy + tailley + decalage_y_scaled < 0)
    return true;
  return false;
}

void calculer_positions_ecran(Object_image *obj, int coeff, int decalage_x,
                              int decalage_y, int *x_ecran, int *y_ecran,
                              int *w_ecran, int *h_ecran) {
  int px, py, tx, ty;
  if (obj->type == TYPE_IMAGE) {
    px = obj->image.posx;
    py = obj->image.posy;
    tx = obj->image.taillex;
    ty = obj->image.tailley;
  } else if (obj->type == TYPE_FORME) {
    px = obj->forme.posx;
    py = obj->forme.posy;
    tx = obj->forme.posx2;
    ty = obj->forme.posy2;
  }
  *x_ecran = (px * coeff) + decalage_x;
  *y_ecran = (py * coeff) + decalage_y;
  *w_ecran = tx * coeff;
  *h_ecran = ty * coeff;
}

void afficher_images(Gestionnaire *gs) {
  if (!gs || !gs->rendu || !gs->image) {
    log_fmt(NiveauLogErreur,
            "Erreur: Composants manquants pour afficher les images");
    return;
  }
  Tableau_image *jeu = gs->image;

  int coeff = gs->largeur_actuel / gs->largeur;
  int decalage_x = gs->decalage_x;
  int decalage_y = gs->decalage_y;
  int decalage_x_scaled = (int)lround((double)gs->decalage_x / (double)coeff);
  int decalage_y_scaled = (int)lround((double)gs->decalage_y / (double)coeff);

  for (int i = 0; i < jeu->nb_images; i++) {
    Object_image *obj = &jeu->tab[i];

    int obj_x = (obj->type == TYPE_IMAGE) ? obj->image.posx : obj->forme.posx;
    int obj_y = (obj->type == TYPE_IMAGE) ? obj->image.posy : obj->forme.posy;
    int obj_w =
        (obj->type == TYPE_IMAGE) ? obj->image.taillex : obj->forme.posx2;
    int obj_h =
        (obj->type == TYPE_IMAGE) ? obj->image.tailley : obj->forme.posy2;

    if (hors_ecran(gs, obj_x, obj_y, obj_w, obj_h, decalage_x_scaled,
                   decalage_y_scaled))
      continue;

    int x_ecran, y_ecran, w_ecran, h_ecran;
    calculer_positions_ecran(obj, coeff, decalage_x, decalage_y, &x_ecran,
                             &y_ecran, &w_ecran, &h_ecran);

    SDL_Rect dst = {x_ecran, y_ecran, w_ecran, h_ecran};

    afficher_objet(gs, obj, &dst, x_ecran, y_ecran, w_ecran, h_ecran, coeff);
  }
  jeu->nb_images = 0;
}