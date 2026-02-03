/*
 * Detection des touches clavier.
 */

#include "../../../main.h"

/* Verifie si une touche clavier vient d'etre pressee (front montant) */
bool touche_juste_presse(const char *touche) {
    if (!gs)
        goto gsvide;
    if (!touche)
        return false;
    /* Conversion du nom de la touche en scancode SDL */
    SDL_Scancode sc = scancode_depuis_nom(touche);

    return (sc != SDL_SCANCODE_UNKNOWN) && gs->entrees->entrees_presse[sc];

gsvide:
    log_message(NiveauLogDebug, "manager empty in key just pressed function");
    return false;
}

/* Verifie si une touche clavier est maintenue enfoncee */
bool touche_enfoncee(const char *touche) {
    if (!gs)
        goto gsvide;
    if (!touche)
        return false;

    /* Conversion du nom de la touche en scancode SDL */
    SDL_Scancode sc = scancode_depuis_nom(touche);
    return (sc != SDL_SCANCODE_UNKNOWN) && gs->entrees->entrees[sc];

gsvide:
    log_message(NiveauLogDebug, "manager empty in key pressed function");
    return false;
}