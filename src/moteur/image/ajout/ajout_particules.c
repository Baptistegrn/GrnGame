/*
 * Ajout de particules a la liste de rendu.
 */

#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../boucle/boucle.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "../affichage/affichage.h"
#include "../particule/particule.h"
#include "ajout.h"
#include <stdbool.h>

/* Ajoute un tableau de particules au tableau d'images */
void ajouter_particules_au_tableau(Particules *P) {
    if (UNLIKELY(!gs))
        goto gsvide;

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein(tab);
    int i = tab->nb_images++;

    /* Affectation directe des pointeurs de tableaux */
    tab->tab[i].type = TYPE_PARTICULE;
    tab->tab[i].particules = P;
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in add particules to array");
}