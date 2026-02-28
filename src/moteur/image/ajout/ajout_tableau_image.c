/*
 * reallocation automatique du tableau dimage
 */

#include "../../../allouer/allouer.h"
#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../boucle/boucle.h"
#include "../../logging/logging.h"
#include "../affichage/affichage.h"
#include "ajout.h"
#include <stdbool.h>

/* reallocation automatique du tableau dimage */
void reallouer_si_plein(TableauImage *jeu) {
    if (UNLIKELY(!gs))
        goto gsvide;
    if (jeu->nb_images >= jeu->capacite_images) {
        int nouvelle_capacite = (jeu->capacite_images == 0) ? 200 : jeu->capacite_images * 2;
        jeu->tab =
            realloc_gestion_echec_compteur(jeu->tab, sizeof(ObjectImage) * nouvelle_capacite);
        jeu->capacite_images = nouvelle_capacite;
    }
    return;
gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in realloction of image table");
}