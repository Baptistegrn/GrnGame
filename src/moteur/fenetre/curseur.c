#include "../../main.h"

/*
 * Affiche ou masque le curseur de la souris selon le paramètre.
 */
void afficher_curseur(bool afficher) {
    if (afficher) {
        SDL_ShowCursor(SDL_ENABLE);
        log_message(NiveauLogDebug, "Cursor enabled");
    } else {
        SDL_ShowCursor(SDL_DISABLE);
        log_message(NiveauLogDebug, "Cursor disabled");
    }
}
