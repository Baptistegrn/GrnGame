/*
 * Ajout d'images et de sprites a la liste de rendu.
 */

#include "../../../allouer/allouer.h"
#include "../../../main.h"
#include "../../../module_jeu/module_jeu.h"
#include "../../../prediction_branche.h"
#include "../../boucle/boucle.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "../affichage/affichage.h"
#include "../chargement/chargement_image.h"
#include "../rotation/rotation.h"
#include "SDL_render.h"
#include "ajout.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>

/* Ajoute un sprite au tableau de rendu avec Culling et affectation directe */
void ajouter_sprite_au_tableau(Sprite *sprite, Sint16 index, float x, float y, Sint16 coeff,
                               bool sens, Uint16 rotation, Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;

    const char *id = sprite->id;

    /* recuperation texture */
    SDL_Texture *texture_base = recuperer_texture_par_lien(id);
    if (!texture_base) {
        log_fmt(NiveauLogErreur, "Base texture not found for sprite '%s'", id);
        return;
    }

    int tex_w, tex_h;
    SDL_QueryTexture(texture_base, NULL, NULL, &tex_w, &tex_h);

    int taille_ecran_x = sprite->taillex * coeff;
    int taille_ecran_y = sprite->tailley * coeff;
    float ecran_x = x - gs->module_jeu->camera->x;
    float ecran_y = y - gs->module_jeu->camera->y;

    int decalage_x = (int)lround((double)gs->fenetre->decalage_x / (double)gs->fenetre->coeff);
    int decalage_y = (int)lround((double)gs->fenetre->decalage_y / (double)gs->fenetre->coeff);

    if (hors_ecran(ecran_x, ecran_y, sprite->taillex, taille_ecran_y, decalage_x, decalage_y)) {
        return;
    }

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
    int source_x1 = (idx % nb_colonnes) * sprite->taillex;
    int source_y1 = (idx / nb_colonnes) * sprite->tailley;

    /* Verification des limites dans la texture source */
    if (UNLIKELY(source_x1 + sprite->taillex > tex_w || source_y1 + sprite->tailley > tex_h)) {
        log_fmt(NiveauLogErreur, "Sprite index %d out of bounds for texture %s", index, id);
        return;
    }

    /* reallocation */
    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    /* Correction rotation */
    if (rotation > 359) {
        rotation = 0;
        log_fmt(NiveauLogAvertissement, "Rotation invalid for sprite %s correction to 0", id);
    }

    tab->tab[i].type = TYPE_IMAGE;
    tab->tab[i].image.texture = texture_base;
    tab->tab[i].image.posx = ecran_x;
    tab->tab[i].image.posy = ecran_y;
    tab->tab[i].image.taillex = taille_ecran_x;
    tab->tab[i].image.tailley = taille_ecran_y;
    tab->tab[i].image.sens = sens;

    /* Les sprites ne beneficient pas du cache de rotation pixel perfect */
    tab->tab[i].image.rotation_p = 0;
    tab->tab[i].image.rotation = rotation;

    /* Limitation des valeurs Alpha */
    tab->tab[i].image.a = SDL_clamp(a, 0, 255);
    tab->tab[i].image.sprite = true;

    tab->tab[i].image.x1 = source_x1;
    tab->tab[i].image.y1 = source_y1;
    tab->tab[i].image.x2 = sprite->taillex;
    tab->tab[i].image.y2 = sprite->tailley;

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

/* Ajoute une image au tableau de rendu avec gestion du cache de rotation et Culling */
void ajouter_image_au_tableau(const char *id, float x, float y, Sint16 coeff, bool sens,
                              Uint16 rotation_p, Uint16 rotation, Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;

    /* Récupération de la texture de base et de sa taille */
    SDL_Texture *tex = recuperer_texture_par_lien(id);
    if (UNLIKELY(!tex)) {
        log_fmt(NiveauLogErreur, "Texture not found for '%s'", id);
        return;
    }

    int taillex, tailley;
    SDL_QueryTexture(tex, NULL, NULL, &taillex, &tailley);

    int decalage_x = (int)lround((double)gs->fenetre->decalage_x / (double)gs->fenetre->coeff);
    int decalage_y = (int)lround((double)gs->fenetre->decalage_y / (double)gs->fenetre->coeff);
    float ecran_x = x - gs->module_jeu->camera->x;
    float ecran_y = y - gs->module_jeu->camera->y;

    /* si hors ecran on ajoute pas */
    if (hors_ecran(ecran_x, ecran_y, taillex, tailley, decalage_x, decalage_y)) {
        return;
    }

    /* reallocation */
    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();

    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_IMAGE;
    tab->tab[i].image.posx = ecran_x;
    tab->tab[i].image.posy = ecran_y;
    tab->tab[i].image.taillex = taillex * coeff;
    tab->tab[i].image.tailley = tailley * coeff;
    tab->tab[i].image.sens = sens;
    /* limitation alpha */
    tab->tab[i].image.a = SDL_clamp(a, 0, 255);

    /* rotation */
    if (rotation_p > 359) {
        rotation_p = 0;
        log_fmt(NiveauLogAvertissement, "Rotation invalid for image %s correction to 0", id);
    }

    tab->tab[i].image.rotation_p = rotation_p;
    tab->tab[i].image.rotation = rotation;

    if (rotation_p == 0) {
        tab->tab[i].image.texture = tex;
    } else {
        tab->tab[i].image.texture = recuperer_texture_variante(id, rotation_p);

        if (!tab->tab[i].image.texture) {
            tab->tab[i].image.texture =
                creer_texture_angle(tex, taillex, tailley, rotation_p, SDL_FLIP_NONE);
            if (tab->tab[i].image.texture) {
                ajouter_variante_cache(id, tab->tab[i].image.texture, rotation_p);
            } else {
                tab->tab[i].image.texture = tex;
            }
        }
    }
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in add image to table");
}