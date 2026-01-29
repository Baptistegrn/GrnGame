#include "../../../main.h"

void afficher_objet(ObjectImage *obj, SDL_Rect *dst, Sint16 x_ecran, Sint16 y_ecran, Sint16 w_ecran,
                    Sint16 h_ecran, unsigned char coeff) {
    if (!gs) {
        goto gsvide;
    }
    // cas image/sprite
    if (obj->type == TYPE_IMAGE) {
        if (!obj->image.texture)
            return;

        // Configuration de la texture
        SDL_SetTextureBlendMode(obj->image.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(obj->image.texture, obj->image.a);
        SDL_SetTextureScaleMode(obj->image.texture, SDL_ScaleModeNearest);

        SDL_Rect src;
        SDL_Rect *pSrc = NULL;

        if (obj->image.sprite) {
            src.x = obj->image.x1;
            src.y = obj->image.y1;
            src.w = obj->image.x2;
            src.h = obj->image.y2;
            pSrc = &src;
        }

        if (obj->image.rotation != 0) {
            ajuster_rect_texture_tournee(obj->image.texture, dst, x_ecran, y_ecran, w_ecran,
                                         h_ecran, coeff);
        }

        // flip
        SDL_RendererFlip flip = (obj->image.sens == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(gs->fenetre->rendu, obj->image.texture, pSrc, dst, 0.0, NULL, flip);

    }
    // forme
    else if (obj->type == TYPE_FORME) {
        SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);

        switch (obj->forme.type_de_forme) {
        case 1: // Ligne
            dessiner_ligne_pixel(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                 obj->forme.tailley, obj->forme.r, obj->forme.g, obj->forme.b);
            break;

        case 2: // Rectangle
            if (obj->forme.plein)
                dessiner_rectangle_plein(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                         obj->forme.tailley, obj->forme.r, obj->forme.g,
                                         obj->forme.b, obj->forme.a);
            else
                dessiner_rectangle_vide(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                        obj->forme.tailley, obj->forme.r, obj->forme.g,
                                        obj->forme.b);
            break;

        case 3: // Cercle
        {
            Sint16 rayon = obj->forme.taillex / 2;
            if (obj->forme.plein)
                dessiner_cercle_plein(obj->forme.posx, obj->forme.posy, rayon, obj->forme.r,
                                      obj->forme.g, obj->forme.b);
            else
                dessiner_cercle_vide(obj->forme.posx, obj->forme.posy, rayon, obj->forme.r,
                                     obj->forme.g, obj->forme.b);
        } break;

        case 4: // Triangle
            if (obj->forme.plein)
                dessiner_triangle_plein(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                        obj->forme.tailley, obj->forme.r, obj->forme.g,
                                        obj->forme.b);
            else
                dessiner_triangle_vide(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                       obj->forme.tailley, obj->forme.r, obj->forme.g,
                                       obj->forme.b);
            break;
        }
    } else if (obj->type == TYPE_PARTICULE) {
        dessiner_particules(&obj->particule);
    }

gsvide:
    log_message(NiveauLogDebug, "empty manager in the function that displays objects");
}