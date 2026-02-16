#include "../../allouer/allouer.h"
#include "../../main.h"
#include "cJSON.h"
#include "json.h"


/* libere le gestionnaire de fichiers */
void liberer_gestionnaire_fichiers(void) {
    if (!gs)
        return;

    GestionnaireFichiers *gf = gs->fichiers;

    for (int i = 0; i < gf->nb_fichiers; i++) {
        if (gf->liste[i].contenu) {
            cJSON_Delete((cJSON *)gf->liste[i].contenu);
        }
    }

    if (gf->liste)
        free_gestion_echec_compteur(gf->liste);
    free_gestion_echec_compteur(gf);
    gs->fichiers = NULL;
}
