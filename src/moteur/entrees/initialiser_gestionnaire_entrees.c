/* initialise le gestionnaire dentrees */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../logging/logging.h"
#include "entrees.h"
#include <string.h>

/* initialise ses sous strucutures */
void initialiser_gestionnaire_entrees(void) {
    gs->entrees = malloc_gestion_echec_compteur(sizeof(GestionnaireEntrees));

    memset(gs->entrees, 0, sizeof(GestionnaireEntrees));

    gs->entrees->texte_ecrit = malloc_gestion_echec_compteur(sizeof(char) * 255);
    gs->entrees->capacite_texte_ecrit = 255;
    gs->entrees->taille_texte_ecrit = 0;

    log_message(NiveauLogDebug, "Allocated inputs");
}