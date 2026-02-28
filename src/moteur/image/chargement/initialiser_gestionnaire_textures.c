/* initialise le gestionnaire de textures */

#include "../../../allouer/allouer.h"
#include "../../../main.h"
#include "../../logging/logging.h"
#include "chargement_image.h"
#include <string.h>

/* initialise les sous structures */
void initialiser_gestionnaire_textures(void) {
    gs->textures = malloc_gestion_echec_compteur(sizeof(GestionnaireTextures));
    memset(gs->textures, 0, sizeof(GestionnaireTextures));

    gs->textures->capacite = 50;
    gs->textures->taille = 0;
    gs->textures->entrees =
        malloc_gestion_echec_compteur(gs->textures->capacite * sizeof(TextureEntry));

    memset(gs->textures->entrees, 0, gs->textures->capacite * sizeof(TextureEntry));
    log_message(NiveauLogDebug, "Allocated texture");
    return;
}