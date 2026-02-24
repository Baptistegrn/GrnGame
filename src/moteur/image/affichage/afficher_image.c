/*
 * Affichage des images et formes a l'ecran.
 */

#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../boucle/boucle.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "SDL_stdinc.h"
#include "affichage.h"

/* Calcule les coordonnees et dimensions d'un objet a l'ecran */
void calculer_positions_ecran(ObjectImage *obj, unsigned char coeff, int decalage_x, int decalage_y,
                              Sint16 *x_ecran, Sint16 *y_ecran, Sint16 *w_ecran, Sint16 *h_ecran) {
    float px, py;
    Sint16 tx, ty;

    if (obj->type == TYPE_IMAGE) {
        px = obj->image.posx;
        py = obj->image.posy;
        tx = obj->image.taillex;
        ty = obj->image.tailley;
    } else if (obj->type == TYPE_FORME) {
        px = obj->forme.posx;
        py = obj->forme.posy;
        tx = obj->forme.taillex;
        ty = obj->forme.tailley;
    }

    *x_ecran = SDL_lroundf(px * (float)coeff + (float)decalage_x);
    *y_ecran = SDL_lroundf(py * (float)coeff + (float)decalage_y);
    *w_ecran = SDL_lroundf((float)tx * (float)coeff);
    *h_ecran = SDL_lroundf((float)ty * (float)coeff);
}

/* Affiche tous les objets (images et formes) de la liste d'affichage */
void afficher_images(void) {
    if (UNLIKELY(!gs))
        goto gsvide;

    TableauImage *tab = gs->frame->image;

    unsigned char coeff = gs->fenetre->coeff;
    int decalage_x = gs->fenetre->decalage_x;
    int decalage_y = gs->fenetre->decalage_y;

    for (int i = 0; i < tab->nb_images; i++) {
        ObjectImage *obj = &tab->tab[i];

        float obj_x = (obj->type == TYPE_IMAGE) ? obj->image.posx : obj->forme.posx;
        float obj_y = (obj->type == TYPE_IMAGE) ? obj->image.posy : obj->forme.posy;
        Sint16 obj_w = (obj->type == TYPE_IMAGE) ? obj->image.taillex : obj->forme.taillex;
        Sint16 obj_h = (obj->type == TYPE_IMAGE) ? obj->image.tailley : obj->forme.tailley;

        bool est_ligne = (obj->type == TYPE_FORME && obj->forme.type_de_forme == 1);

        Sint16 x_ecran, y_ecran, w_ecran, h_ecran;

        calculer_positions_ecran(obj, coeff, decalage_x, decalage_y, &x_ecran, &y_ecran, &w_ecran,
                                 &h_ecran);

        SDL_Rect dst = {x_ecran, y_ecran, w_ecran, h_ecran};

        afficher_objet(obj, &dst, x_ecran, y_ecran, w_ecran, h_ecran, coeff);
    }

    tab->nb_images = 0;

    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in the function that displays images");
}