/*
 * Ajout de particules a la liste de rendu.
 */

#include "../../../main.h"
#include "../../../module_jeu/camera/camera.h"
#include "../../../prediction_branche.h"
#include "../../boucle/boucle.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "../affichage/affichage.h"
#include "ajout.h"
#include <stdbool.h>

/* Ajoute un tableau de particules au tableau d'images */
void ajouter_particules_au_tableau(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r,
                                   Uint8 *g, Uint8 *b, int taille) {
    if (UNLIKELY(!gs))
        goto gsvide;

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    /* Affectation directe des pointeurs de tableaux */
    tab->tab[i].type = TYPE_PARTICULE;
    tab->tab[i].particule.posx = x;
    tab->tab[i].particule.posy = y;
    tab->tab[i].particule.rotation = rotation;
    tab->tab[i].particule.a = a;
    tab->tab[i].particule.r = r;
    tab->tab[i].particule.g = g;
    tab->tab[i].particule.b = b;
    tab->tab[i].particule.taille = taille;

    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in add particules to array");
}