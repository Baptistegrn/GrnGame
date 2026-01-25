#include "../../../main.h"

/*
 * Initialise un contrôleur de jeu et son joystick associé via son index.
 * Vérifie la disponibilité et la compatibilité du périphérique avant
 * l'ouverture.
 */
bool init_controller_joysticks(unsigned char index) {
    if (!gs)
        goto gsvide;
    /* Vérification de l'existence de la manette */
    if (SDL_NumJoysticks() <= index) {
        log_fmt(NiveauLogErreur, "Error No controller available at index %d", index);
        return false;
    }

    /* Vérification de la compatibilité GameController */
    if (!SDL_IsGameController(index)) {
        log_fmt(NiveauLogErreur, "Error Unable to recognize device %d as a controller", index);
        return false;
    }

    /* Ouverture du GameController */
    SDL_GameController *controller = SDL_GameControllerOpen(index);
    if (!controller) {
        log_fmt(NiveauLogErreur, "Error Unable to open controller %d: %s", index, SDL_GetError());
        return false;
    }

    /* Ouverture du Joystick brut associé */
    SDL_Joystick *joy = SDL_JoystickOpen(index);
    if (!joy) {
        log_fmt(NiveauLogErreur, "Error Unable to open joystick %d: %s", index, SDL_GetError());
        SDL_GameControllerClose(controller);
        return false;
    }

    /* Assignation au gestionnaire global */
    gs->entrees->man[index].manette = controller;
    gs->entrees->man[index].joystick = joy;
    log_fmt(NiveauLogInfo, "Controller %d opened: %s", index, SDL_GameControllerName(controller));
    return true;

gsvide:
    log_message(NiveauLogDebug, "manager empty in initialize controller function");
    return false;
}