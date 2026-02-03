/*
 * Ajout d'images au tableau dynamique de rendu.
 */

#include "../../../main.h"
#include "ajout.h"

/* Ajoute une image au tableau dynamique avec reallocation automatique */
void ajouter_image_au_jeu(ObjectImage nouvelle) {
    if (!gs)
        goto gsvide;
    TableauImage *jeu = gs->frame->image;

    /* Extension de la capacite si necessaire */
    if (jeu->nb_images >= jeu->capacite_images) {
        int nouvelle_capacite = (jeu->capacite_images == 0) ? 100 : jeu->capacite_images + 50;
        jeu->tab =
            realloc_gestion_echec_compteur(jeu->tab, sizeof(ObjectImage) * nouvelle_capacite);
        jeu->capacite_images = nouvelle_capacite;
    }

    /* Ajout de la nouvelle image */
    jeu->tab[jeu->nb_images++] = nouvelle;

    return;
gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in add of image structure to table");
}