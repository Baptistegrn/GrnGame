/* initialise le gestionnaire dentrees */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../logging/logging.h"
#include "entrees.h"
#include <string.h>

void initialiser_gestionnaire_entrees(void) {
    gs->entrees = malloc_gestion_echec_compteur(sizeof(GestionnaireEntrees));
    memset(gs->entrees, 0, sizeof(GestionnaireEntrees));
    log_message(NiveauLogDebug, "Allocated inputs");
}