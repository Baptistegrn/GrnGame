/* remet a 0 les clefs de chiffrements */

#include "../../main.h"

void mettre_cle_vide() {
    if (!gs)
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