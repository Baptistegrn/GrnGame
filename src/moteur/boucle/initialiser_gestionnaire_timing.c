/* initialise le gestionnaire de temp */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../../proprietes.h"
#include "../initialisation/initialisation.h"
#include "../logging/logging.h"
#include "boucle.h"
#include <string.h>

/* initialise chaques sous structures */
void initialiser_gestionnaire_timing() {
    gs->timing = malloc_gestion_echec_compteur(sizeof(GestionnaireTiming));
    memset(gs->timing, 0, sizeof(GestionnaireTiming));
    gs->timing->run = true;
    gs->timing->en_pause = false;
    gs->timing->minimise = false;
    gs->timing->dt = 0.0001;
    gs->timing->fps = DEFAULT_FPS;
    log_message(NiveauLogDebug, "Allocated timing");
}