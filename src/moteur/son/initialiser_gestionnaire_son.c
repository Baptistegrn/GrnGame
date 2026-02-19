/* initialise le gestionnaire de sons */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../logging/logging.h"
#include "son.h"
#include <string.h>

void initialiser_gestionnaire_son(void) {
    gs->sons = malloc_gestion_echec_compteur(sizeof(GestionnaireSon));
    memset(gs->sons, 0, sizeof(GestionnaireSon));
    gs->sons->capacite = 50;
    gs->sons->taille = 0;

    gs->sons->entrees = malloc_gestion_echec_compteur(sizeof(SonEntry) * gs->sons->capacite);

    memset(gs->sons->entrees, 0, sizeof(SonEntry) * gs->sons->capacite);
    log_message(NiveauLogDebug, "Allocated song");
    return;
}