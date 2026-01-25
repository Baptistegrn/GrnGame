#include "../../main.h"

/*
 * Définit l'icône de la fenêtre SDL en chargeant une image depuis un fichier.
 * L'image est chargée, appliquée à la fenêtre, puis libérée de la mémoire.
 */
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
