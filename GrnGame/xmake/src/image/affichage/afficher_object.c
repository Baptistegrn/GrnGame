#include "../../main.h"
#include <math.h>

void afficher_objet(Gestionnaire *gs, Object_image *obj, SDL_Rect *dst,
                    int x_ecran, int y_ecran, int w_ecran, int h_ecran,
                    int coeff) {
  if (obj->type == TYPE_IMAGE) {
    if (!obj->image.texture)
      return;

    SDL_SetTextureBlendMode(obj->image.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(obj->image.texture, obj->image.transparence);
    SDL_SetTextureScaleMode(obj->image.texture, SDL_ScaleModeNearest);

    if (obj->image.rotation != 0) {
      ajuster_rect_texture_tournee(obj->image.texture, dst, x_ecran, y_ecran,
                                   w_ecran, h_ecran, coeff);
    }

    if (obj->image.rotation == 0 && obj->image.sens == 1) {
      SDL_RenderCopyEx(gs->rendu, obj->image.texture, NULL, dst, 0.0, NULL,
                       SDL_FLIP_HORIZONTAL);
    } else {
      SDL_RenderCopy(gs->rendu, obj->image.texture, NULL, dst);
    }

    SDL_SetTextureBlendMode(obj->image.texture, SDL_BLENDMODE_NONE);
  } else if (obj->type == TYPE_FORME) {
    SDL_SetRenderDrawBlendMode(gs->rendu, SDL_BLENDMODE_NONE);

    if (obj->forme.type_de_forme == 1) { // Ligne
      dessiner_ligne_pixel(gs, obj->forme.posx, obj->forme.posy,
                           obj->forme.posx2, obj->forme.posy2, obj->forme.r,
                           obj->forme.g, obj->forme.b);
    } else if (obj->forme.type_de_forme == 2) { // Rectangle
      if (obj->forme.plein) {
        dessiner_rectangle_plein(gs, obj->forme.posx, obj->forme.posy,
                                 obj->forme.posx2, obj->forme.posy2,
                                 obj->forme.r, obj->forme.g, obj->forme.b);
      } else {
        dessiner_rectangle_vide(gs, obj->forme.posx, obj->forme.posy,
                                obj->forme.posx2, obj->forme.posy2,
                                obj->forme.r, obj->forme.g, obj->forme.b);
      }
    } else if (obj->forme.type_de_forme == 3) { // Cercle
      int centre_x = obj->forme.posx;
      int centre_y = obj->forme.posy;
      int rayon = obj->forme.posx2 / 2;

      if (obj->forme.plein) {
        dessiner_cercle_plein(gs, centre_x, centre_y, rayon, obj->forme.r,
                              obj->forme.g, obj->forme.b);
      } else {
        dessiner_cercle_vide(gs, centre_x, centre_y, rayon, obj->forme.r,
                             obj->forme.g, obj->forme.b);
      }
    } else if (obj->forme.type_de_forme == 4) { // Triangle
      if (obj->forme.plein) {
        dessiner_triangle_plein(gs, obj->forme.posx, obj->forme.posy,
                                obj->forme.posx2, obj->forme.posy2,
                                obj->forme.r, obj->forme.g, obj->forme.b);
      } else {
        dessiner_triangle_vide(gs, obj->forme.posx, obj->forme.posy,
                               obj->forme.posx2, obj->forme.posy2, obj->forme.r,
                               obj->forme.g, obj->forme.b);
      }
    }
  }
}