#include "../../logging/logging.h"
#include "../../main.h"
#include <math.h>
#include <stdlib.h>


void ajouter_image_au_jeu(Gestionnaire *gs, Object_image nouvelle) {
  if (!gs || !gs->image) {
    log_fmt(NiveauLogErreur, "Erreur: Gestionnaire ou tableau d'images NULL");
    return;
  }

  Tableau_image *jeu = gs->image;

  if (jeu->nb_images >= jeu->capacite_images) {
    int nouvelle_capacite =
        (jeu->capacite_images == 0) ? 100 : jeu->capacite_images + 50;
    Object_image *nouveau_tab =
        realloc(jeu->tab, sizeof(Object_image) * nouvelle_capacite);

    if (!nouveau_tab) {
      log_fmt(NiveauLogErreur, "Erreur: Échec de réallocation mémoire");
      return;
    }

    jeu->tab = nouveau_tab;
    jeu->capacite_images = nouvelle_capacite;
  }

  jeu->tab[jeu->nb_images++] = nouvelle;
}