/* remet a 0 les clefs de chiffrements */

#include "../../main.h"
#include "../../prediction_branche.h"
#include "../image/chargement/chargement_image.h"
#include "../json/json.h"
#include "../logging/logging.h"
#include "../son/son.h"
#include <string.h>

/* renetialise la cle de chiffrements a 0 pour chaques cles */
void mettre_cle_vide() {
    if (UNLIKELY(!gs))
        goto gsvide;

    /* cle vide */
    memset(gs->textures->cle, 0, 16);
    memset(gs->textures->iv, 0, 16);

    /* cle vide */
    memset(gs->sons->cle, 0, 16);
    memset(gs->sons->iv, 0, 16);

    /* cle vide */
    memset(gs->fichiers->cle, 0, 16);
    memset(gs->fichiers->iv, 0, 16);
    return;
gsvide:
    log_message(NiveauLogErreur, "manager empty in update crypted key function");
}