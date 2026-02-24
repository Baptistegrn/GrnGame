/*
 * Affichage des objets individuels (images, formes, particules).
 */

#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "../dessin_forme/dessin.h"
#include "../rotation/rotation.h"
#include "SDL_stdinc.h"
#include "affichage.h"

/* Affiche un objet selon son type (image, forme ou particule) */
void afficher_objet(ObjectImage *obj, SDL_Rect *dst, Sint16 x_ecran, Sint16 y_ecran, Sint16 w_ecran,
                    Sint16 h_ecran, unsigned char coeff) {
    if (UNLIKELY(!gs))
        goto gsvide;

    /* cas image/sprite */
    if (obj->type == TYPE_IMAGE) {
        if (!obj->image.texture)
            return;

        /* Configuration de la texture */
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

        if (obj->image.rotation != 0 && !obj->image.sprite) {
            /* bug sprite */
            ajuster_rect_texture_tournee(obj->image.texture, dst, x_ecran, y_ecran, w_ecran,
                                         h_ecran, coeff);
        }

        /* flip */
        SDL_RendererFlip flip = (obj->image.sens == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(gs->fenetre->rendu, obj->image.texture, pSrc, dst, obj->image.rotation,
                         NULL, flip);

    }
    /* forme */
    else if (obj->type == TYPE_FORME) {
        SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);

        switch (obj->forme.type_de_forme) {
        case 1: /* Ligne */
            if (obj->forme.posy == obj->forme.posy2) {
                /* Ligne horizontale parfaite */
                float x_start =
                    obj->forme.posx < obj->forme.posx2 ? obj->forme.posx : obj->forme.posx2;
                float longueur = fabsf(obj->forme.posx2 - obj->forme.posx);

                tracer_ligne_horizontale_float(x_start, obj->forme.posy, longueur, obj->forme.r,
                                               obj->forme.g, obj->forme.b, obj->forme.a);
            } else if (obj->forme.posx == obj->forme.posx2) {
                /* Ligne verticale parfaite */
                float y_start =
                    obj->forme.posy < obj->forme.posy2 ? obj->forme.posy : obj->forme.posy2;
                float longueur = fabsf(obj->forme.posy2 - obj->forme.posy);

                tracer_ligne_verticale_float(obj->forme.posx, y_start, longueur, obj->forme.r,
                                             obj->forme.g, obj->forme.b, obj->forme.a);
            } else {
                /* ligne non droite */
                dessiner_ligne_pixel(obj->forme.posx, obj->forme.posy, obj->forme.posx2,
                                     obj->forme.posy2, obj->forme.r, obj->forme.g, obj->forme.b,
                                     obj->forme.a);
            }
            break;

        case 2: /* Rectangle */
            if (obj->forme.plein)
                dessiner_rectangle_plein(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                         obj->forme.tailley, obj->forme.r, obj->forme.g,
                                         obj->forme.b, obj->forme.a);
            else
                dessiner_rectangle_vide(obj->forme.posx, obj->forme.posy, obj->forme.taillex,
                                        obj->forme.tailley, obj->forme.r, obj->forme.g,
                                        obj->forme.b, obj->forme.a);
            break;

        case 3: /* Cercle */
        {
            Sint16 rayon = obj->forme.taillex / 2;
            if (obj->forme.plein)
                dessiner_cercle_plein(obj->forme.posx, obj->forme.posy, rayon, obj->forme.r,
                                      obj->forme.g, obj->forme.b, obj->forme.a);
            else
                dessiner_cercle_vide(obj->forme.posx, obj->forme.posy, rayon, obj->forme.r,
                                     obj->forme.g, obj->forme.b, obj->forme.a);
        } break;
        }
    } else if (obj->type == TYPE_PARTICULE) {
        dessiner_particules(&obj->particule);
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in the function that displays objects");
}