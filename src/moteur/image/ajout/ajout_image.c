/*
 * Ajout d'images et de sprites a la liste de rendu.
 */

#include "../../../main.h"
#include "ajout.h"

/* Ajoute une image au tableau de rendu avec gestion du cache de rotation */
void ajouter_image_au_tableau(const char *id, float x, float y, Sint16 taillex, Sint16 tailley,
                              bool sens, Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;
    /* Correction rotation */
    if (rotation > 359) {
        rotation = 0;

        log_fmt(NiveauLogAvertissement, "Rotation invalid for image %s correction to 0", id);
    }

    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs Alpha entre 0 et 255 */
    a = SDL_clamp(a, 0, 255);
    obj.type = TYPE_IMAGE;
    obj.image.posx = x;
    obj.image.posy = y;
    obj.image.taillex = taillex;
    obj.image.tailley = tailley;
    obj.image.sens = sens;
    obj.image.rotation = rotation;
    obj.image.a = a;

    /* Gestion du cache d'angle */
    if (rotation == 0) {
        obj.image.texture = recuperer_texture_par_lien(id);
    } else {
        obj.image.texture = recuperer_texture_variante(id, rotation);

        if (!obj.image.texture) {
            SDL_Texture *texture_base = recuperer_texture_par_lien(id);

            if (texture_base) {
                SDL_RendererFlip flip = (sens == 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

                obj.image.texture =
                    creer_texture_tournee_rapide(texture_base, taillex, tailley, rotation, flip);

                if (obj.image.texture) {
                    ajouter_variante_cache(

                        id, obj.image.texture, rotation);
                }
            }
        }
    }

    /* Verification texture finale */
    if (!obj.image.texture) {
        log_fmt(NiveauLogErreur, "Texture not found for '%s'", id);
        return;
    }

    ajouter_image_au_jeu(obj);

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add image to image table");
}

/* Ajoute un sprite au tableau de rendu */
void ajouter_sprite_au_tableau(Sprite *sprite, Sint16 index, float x, float y, Sint16 taillex,
                               Sint16 tailley, bool sens, Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;

    /* Correction rotation */
    const char *id = sprite->id;
    if (rotation > 359) {
        rotation = 0;
        log_fmt(NiveauLogAvertissement, "Rotation invalid for image %s correction to 0", id);
    }

    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs Alpha */
    a = SDL_clamp(a, 0, 255);
    obj.type = TYPE_IMAGE;
    obj.image.posx = x;
    obj.image.posy = y;
    obj.image.taillex = taillex;
    obj.image.tailley = tailley;
    obj.image.sens = sens;
    obj.image.rotation = rotation;
    obj.image.a = a;
    obj.image.sprite = true;

    /* Recuperation de la texture */
    if (rotation == 0) {
        obj.image.texture = recuperer_texture_par_lien(id);
    } else {
        obj.image.texture = recuperer_texture_variante(id, rotation);
        if (!obj.image.texture) {
            SDL_Texture *texture_base = recuperer_texture_par_lien(id);
            if (texture_base) {
                SDL_RendererFlip flip = (sens == 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                obj.image.texture =
                    creer_texture_tournee_rapide(texture_base, taillex, tailley, rotation, flip);
                if (obj.image.texture) {
                    ajouter_variante_cache(id, obj.image.texture, rotation);
                }
            }
        }
    }

    if (!obj.image.texture) {
        log_fmt(NiveauLogErreur, "Texture not found for '%s'", id);
        return;
    }

    int tex_w, tex_h;
    SDL_QueryTexture(obj.image.texture, NULL, NULL, &tex_w, &tex_h);
    /* index 0 */
    int idx = index - 1;
    if (idx < 0)
        idx = 0;

    /* Nombre de colonnes dans la texture */
    int nb_colonnes = tex_w / sprite->taillex;
    if (nb_colonnes <= 0) {
        log_message(NiveauLogErreur, "Sprite width is larger than texture width");
        return;
    }
    /* x1, y1 = point haut-gauche | x2, y2 = largeur et hauteur de la decoupe */
    obj.image.x1 = (idx % nb_colonnes) * sprite->taillex;
    obj.image.y1 = (idx / nb_colonnes) * sprite->tailley;
    obj.image.x2 = sprite->taillex;
    obj.image.y2 = sprite->tailley;

    /* Verification des limites */
    if (obj.image.x1 + obj.image.x2 > tex_w || obj.image.y1 + obj.image.y2 > tex_h) {
        log_fmt(NiveauLogErreur, "Sprite index %d out of bounds for texture %s", index, id);
        return;
    }

    ajouter_image_au_jeu(obj);
    return;
gsvide:
    log_message(NiveauLogDebug, "manager is empty in add sprite to image table");
}

/* Cree une structure Sprite avec les dimensions specifiees */
Sprite *creer_sprite(const char *id, Sint16 taillex, Sint16 tailley) {
    Sprite *sprite = malloc_gestion_echec_compteur(sizeof(Sprite));
    sprite->id = strdup(id);
    sprite->taillex = taillex;
    sprite->tailley = tailley;
    return sprite;
}