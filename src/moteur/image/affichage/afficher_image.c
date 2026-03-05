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
    float px = 0;
    float py = 0;
    Sint16 tx = 0;
    Sint16 ty = 0;

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
void afficher_images(TableauImage *tab) {
    if (UNLIKELY(!gs))
        goto gsvide;

    unsigned char coeff = gs->fenetre->coeff;
    int decalage_x = gs->fenetre->decalage_x;
    int decalage_y = gs->fenetre->decalage_y;

    for (int i = 0; i < tab->nb_images; i++) {
        ObjectImage *obj = &tab->tab[i];

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