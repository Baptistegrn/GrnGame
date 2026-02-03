/*
 * Gestion de l'icone de la fenetre.
 */

#include "../../main.h"

/* Definit l'icone de la fenetre SDL en chargeant une image */
void definir_icone(const char *chemin) {
    if (!gs)
        goto gsvide;
    SDL_Window *window = gs->fenetre->fenetre;
    SDL_Surface *icon = IMG_Load(chemin);

    if (icon == NULL) {
        log_fmt(NiveauLogErreur, "impossible to loading icon: %s", IMG_GetError());
        return;
    }

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    log_message(NiveauLogDebug, "Icon successfully applied");
    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in icon function");
}
