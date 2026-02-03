/*
 * Fermeture des peripheriques manette.
 */

#include "../../../main.h"

/* Ferme le controleur de jeu (GameController) s'il est actif */
void fermer_controller(unsigned char index) {
    if (!gs)
        goto gsvide;
    if (gs->entrees->man[index].manette) {
        SDL_GameControllerClose(gs->entrees->man[index].manette);
        gs->entrees->man[index].manette = NULL;
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "manager empty in close controller function");
}

/* Ferme le joystick s'il est actif */
void fermer_joystick(unsigned char index) {
    if (!gs)
        goto gsvide;
    if (gs->entrees->man[index].joystick) {
        SDL_JoystickClose(gs->entrees->man[index].joystick);
        gs->entrees->man[index].joystick = NULL;
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "manager empty in close joystick function");
}