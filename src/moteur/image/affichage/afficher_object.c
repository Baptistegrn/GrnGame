#include "../../../main.h"

/*
 * Affiche un objet spécifique (image ou forme) à l'écran.
 * Gère les textures, transparence et rotation pour les images.
 * Dispatche vers fonctions de dessin primitif pour les formes.
 */
void afficher_objet(ObjectImage *obj, SDL_Rect *dst, Sint16 x_ecran, Sint16 y_ecran, Sint16 w_ecran,
                    Sint16 h_ecran, unsigned char coeff) {
    if (!gs)
        goto gsvide;
    if (obj->type == TYPE_IMAGE) {
        if (!obj->image.texture)
            return;

        SDL_SetTextureBlendMode(obj->image.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(obj->image.texture, obj->image.a);
        SDL_SetTextureScaleMode(obj->image.texture, SDL_ScaleModeNearest);

        if (obj->image.rotation != 0) {
            ajuster_rect_texture_tournee(obj->image.texture, dst, x_ecran, y_ecran, w_ecran,
                                         h_ecran, coeff);
        }

        if (obj->image.rotation == 0 && obj->image.sens == 1) {
            SDL_RenderCopyEx(gs->fenetre->rendu, obj->image.texture, NULL, dst, 0.0, NULL,
                             SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopy(gs->fenetre->rendu, obj->image.texture, NULL, dst);
        }

    } else if (obj->type == TYPE_FORME) {
        SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);

        if (obj->forme.type_de_forme == 1) {
            dessiner_ligne_pixel(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                 obj->forme.tailley, obj->forme.r, obj->forme.g, obj->forme.b);

        } else if (obj->forme.type_de_forme == 2) {
            if (obj->forme.plein) {
                dessiner_rectangle_plein(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                         obj->forme.tailley, obj->forme.r, obj->forme.g,
                                         obj->forme.b, obj->forme.a);
            } else {
                dessiner_rectangle_vide(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                        obj->forme.tailley, obj->forme.r, obj->forme.g,
                                        obj->forme.b);
            }

        } else if (obj->forme.type_de_forme == 3) {
            Sint16 centre_x = obj->forme.posx;
            Sint16 centre_y = obj->forme.posy;
            Sint16 rayon = obj->forme.taillex / 2;

            if (obj->forme.plein) {
                dessiner_cercle_plein(centre_x, centre_y, rayon, obj->forme.r, obj->forme.g,
                                      obj->forme.b);
            } else {
                dessiner_cercle_vide(centre_x, centre_y, rayon, obj->forme.r, obj->forme.g,
                                     obj->forme.b);
            }

        } else if (obj->forme.type_de_forme == 4) {
            if (obj->forme.plein) {
                dessiner_triangle_plein(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                        obj->forme.tailley, obj->forme.r, obj->forme.g,
                                        obj->forme.b);
            } else {
                dessiner_triangle_vide(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                       obj->forme.tailley, obj->forme.r, obj->forme.g,
                                       obj->forme.b);
            }
        }
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in the function that displays objects");
    return;
}