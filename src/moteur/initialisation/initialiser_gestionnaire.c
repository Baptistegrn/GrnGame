/* alloue le gestionnaire */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../boucle/boucle.h"
#include "../fenetre/fenetre.h"
#include "../logging/logging.h"
#include <string.h>

Gestionnaire *gs = NULL;

/* alloue le gestionnaire global */
void initialiser_gestionnaire(void) {
    gs = malloc_gestion_echec_compteur(sizeof(Gestionnaire));
    memset(gs, 0, sizeof(Gestionnaire));
    log_message(NiveauLogDebug, "Allocated manager");
}
