#include "../../../main.h"

/*
 * Vérifie si une touche clavier vient d'être pressée (Front montant).
 * Utilise le nom de la touche (ex: "A", "Space", "Left").
 */
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

/*
 * Vérifie si une touche clavier est maintenue enfoncée.
 */
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