/* initialise le gestionnaire de frame */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../fenetre/fenetre.h"
#include "../image/affichage/affichage.h"
#include "../initialisation/initialisation.h"
#include "../logging/logging.h"
#include "boucle.h"
#include <string.h>

void initialiser_gestionnaire_frame() {
    gs->frame = malloc_gestion_echec_compteur(sizeof(GestionnaireFrameActuelle));
    memset(gs->frame, 0, sizeof(GestionnaireFrameActuelle));

    gs->frame->fond = malloc_gestion_echec_compteur(sizeof(fondActualiser));
    memset(gs->frame->fond, 0, sizeof(fondActualiser));

    gs->frame->image = malloc_gestion_echec_compteur(sizeof(TableauImage));
    memset(gs->frame->image, 0, sizeof(TableauImage));
    gs->frame->image->capacite_images = 10;
    gs->frame->image->nb_images = 0;
    gs->frame->image->tab =
        malloc_gestion_echec_compteur(sizeof(ObjectImage) * gs->frame->image->capacite_images);
    memset(gs->frame->image->tab, 0, sizeof(ObjectImage) * gs->frame->image->capacite_images);
    log_message(NiveauLogDebug, "Allocated frame");
}