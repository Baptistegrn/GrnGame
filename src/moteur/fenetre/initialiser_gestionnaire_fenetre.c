/* initialise le gestionnaire de fenetre et met les parametres pour la creer */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../../prediction_branche.h"
#include "../../proprietes.h"
#include "../logging/logging.h"
#include "fenetre.h"
#include <string.h>

/* initialise ses sous structures */
void initialiser_gestionnaire_fenetre(void) {
    gs->fenetre = malloc_gestion_echec_compteur(sizeof(GestionnaireFenetre));
    memset(gs->fenetre, 0, sizeof(GestionnaireFenetre));
    gs->fenetre->hauteur_univers = DEFAULT_HEIGHT;
    gs->fenetre->largeur_univers = DEFAULT_WIDTH;
    gs->fenetre->nom_fenetre = DEFAULT_WINDOW_TITLE;
    log_message(NiveauLogDebug, "Allocated window");
}