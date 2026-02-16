/*
 * Ajout d'images et de sprites a la liste de rendu.
 */

#include "../../../allouer/allouer.h"
#include "../../../main.h"
#include "../../logging/logging.h"
#include "../chargement/chargement_image.h"
#include "../rotation/rotation.h"
#include "ajout.h"
#include <stdbool.h>
#include <string.h>

/* Ajoute une image au tableau de rendu avec gestion du cache de rotation */
void ajouter_image_au_tableau(const char *id, float x, float y, Sint16 coeff, bool sens,
                              Uint16 rotation_p, Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;
    /* Correction rotation */
    if (rotation_p > 359) {
        rotation_p = 0;

        log_fmt(NiveauLogAvertissement, "Rotation invalid for image %s correction to 0", id);
    }

    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs Alpha entre 0 et 255 */

    a = SDL_clamp(a, 0, 255);
    obj.type = TYPE_IMAGE;
    obj.image.posx = x;
    obj.image.posy = y;
    obj.image.sens = sens;
    obj.image.rotation_p = rotation_p;
    obj.image.rotation = rotation;
    obj.image.a = a;
    /* taille de la texture */
    int taillex, tailley;
    /* Gestion du cache d'angle */
    if (rotation_p == 0) {
        obj.image.texture = recuperer_texture_par_lien(id);
        if (obj.image.texture) /* recuperer la taille de la texture */
            SDL_QueryTexture(obj.image.texture, NULL, NULL, &taillex, &tailley);
    } else {
        obj.image.texture = recuperer_texture_variante(id, rotation_p);

        if (!obj.image.texture) {
            SDL_Texture *texture_base = recuperer_texture_par_lien(id);
            if (texture_base) {
                /* recuperer la taille de la texture */
                SDL_QueryTexture(texture_base, NULL, NULL, &taillex, &tailley);
                obj.image.texture =
                    creer_texture_angle(texture_base, taillex, tailley, rotation_p, SDL_FLIP_NONE);

                if (obj.image.texture) {
                    ajouter_variante_cache(

                        id, obj.image.texture, rotation_p);
                }
            }
        }
    }
    /* gestion du coeff de la texture */
    obj.image.taillex = taillex * coeff;
    obj.image.tailley = tailley * coeff;

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
void ajouter_sprite_au_tableau(Sprite *sprite, Sint16 index, float x, float y, Sint16 coeff,
                               bool sens, Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;

    const char *id = sprite->id;

    /* Correction rotation */
    if (rotation > 359) {
        rotation = 0;
        log_fmt(NiveauLogAvertissement, "Rotation invalid for sprite %s correction to 0", id);
    }

    /* Recuperer la texture de base AVANT l'init de l'objet */
    SDL_Texture *texture_base = recuperer_texture_par_lien(id);
    if (!texture_base) {
        log_fmt(NiveauLogErreur, "Base texture not found for sprite '%s'", id);
        return;
    }

    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs Alpha */
    a = SDL_clamp(a, 0, 255);

    obj.type = TYPE_IMAGE;
    /* Assignation de la texture recuperee plus haut */
    obj.image.texture = texture_base;
    obj.image.posx = x;
    obj.image.posy = y;
    obj.image.taillex = sprite->taillex * coeff;
    obj.image.tailley = sprite->tailley * coeff;
    obj.image.sens = sens;
    /* Les sprites ne beneficient pas du cache de rotation pixel perfect */
    obj.image.rotation_p = 0;
    obj.image.rotation = rotation;
    obj.image.a = a;
    obj.image.sprite = true;

    int tex_w, tex_h;
    SDL_QueryTexture(texture_base, NULL, NULL, &tex_w, &tex_h);

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
    Sprite *sprite = (Sprite *)malloc_gestion_echec_compteur(sizeof(Sprite));
    strncpy((char *)sprite->id, id, TAILLE_LIEN);
    ((char *)sprite->id)[TAILLE_LIEN - 1] = '\0';
    sprite->taillex = taillex;
    sprite->tailley = tailley;
    return sprite;
}