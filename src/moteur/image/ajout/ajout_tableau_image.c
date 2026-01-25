#include "../../../main.h"
#include "ajout.h"

/*
 * Ajoute une image au tableau dynamique avec réallocation automatique.
 * Gère l'extension de capacité et l'ajout du nouvel objet.
 */
void ajouter_image_au_jeu(ObjectImage nouvelle) {
    if (!gs)
        goto gsvide;
    TableauImage *jeu = gs->image;

    /* Extension de la capacité si nécessaire */
    if (jeu->nb_images >= jeu->capacite_images) {
        int nouvelle_capacite = (jeu->capacite_images == 0) ? 100 : jeu->capacite_images + 50;
        jeu->tab = xrealloc(jeu->tab, sizeof(ObjectImage) * nouvelle_capacite);
        jeu->capacite_images = nouvelle_capacite;
    }

    /* Ajout de la nouvelle image */
    jeu->tab[jeu->nb_images++] = nouvelle;

    return;
gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in add of image structure to table");
}