#include "../../main.h"
#include <math.h>
#include <string.h>

/*
 * Trouve l'index local (0 à NB_MANETTES_MAX-1) d'une manette à partir de
 * son instance ID SDL. Retourne -1 si non trouvée.
 */
static int trouver_index_manette(SDL_JoystickID instance_id) {
    SDL_GameController *ctrl = SDL_GameControllerFromInstanceID(instance_id);
    if (!ctrl)
        return -1;

    for (int i = 0; i < NB_MANETTES_MAX; i++) {
        if (gs->entrees->man[i].manette == ctrl) {
            return i;
        }
    }
    return -1;
}

/*
 * Met à jour les entrées (Clavier, Souris, Manette) pour la frame actuelle.
 * Réinitialise les états "juste pressé", récupère la position souris corrigée
 * en tenant compte du letterboxing, puis traite la file d'événements SDL.
 */
void mise_a_jour_input() {
    if (!gs)
        goto gsvide;
    GestionnaireEntrees *entrees = gs->entrees;
    GestionnaireFenetre *fenetre = gs->fenetre;
    SDL_Event event;

    /* Réinitialisation des états "Just Pressed" (durée 1 frame) */
    entrees->souris_juste_presse = false;
    entrees->souris_droite_juste_presse = false;

    memset(entrees->entrees_presse, false, sizeof(entrees->entrees_presse));
    for (int i = 0; i < NB_MANETTES_MAX; i++) {
        for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; j++) {
            entrees->manette_presse[j][i] = false;
        }
    }

    /* Traitement de la file d'événements SDL */
    while (SDL_PollEvent(&event)) {

        /* Mise à jour de la position souris avec correction du letterboxing.
           Les coordonnées écran (physiques) sont converties en coordonnées jeu
           (logiques) en tenant compte du ratio et des bandes noires. */
        int raw_x, raw_y;
        SDL_GetMouseState(&raw_x, &raw_y);

        int coeff = fenetre->coeff;

        entrees->souris_x =
            (int)lroundf(((float)raw_x - (float)fenetre->decalage_x) / (float)coeff);

        entrees->souris_y =
            (int)lroundf(((float)raw_y - (float)fenetre->decalage_y) / (float)coeff);

        /* Dispatch des événements */
        switch (event.type) {

        case SDL_QUIT:
            gs->timing->run = false;
            break;

        /* Manette : Boutons */
        case SDL_CONTROLLERBUTTONDOWN: {
            int idx = trouver_index_manette(event.cbutton.which);
            if (idx >= 0 && event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX) {
                entrees->manette[event.cbutton.button][idx] = true;
                entrees->manette_presse[event.cbutton.button][idx] = true;
            }
            break;
        }

        case SDL_CONTROLLERBUTTONUP: {
            int idx = trouver_index_manette(event.cbutton.which);
            if (idx >= 0 && event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX) {
                entrees->manette[event.cbutton.button][idx] = false;
            }
            break;
        }

        /* Manette : Axes */
        case SDL_CONTROLLERAXISMOTION: {
            int idx = trouver_index_manette(event.caxis.which);
            if (idx < 0)
                break;

            switch (event.caxis.axis) {
            case SDL_CONTROLLER_AXIS_LEFTX:
                entrees->Joy[idx].g.x = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                entrees->Joy[idx].g.y = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                entrees->Joy[idx].d.x = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                entrees->Joy[idx].d.y = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                entrees->trigger[idx].trigger_g = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                entrees->trigger[idx].trigger_d = event.caxis.value;
                break;
            }
            break;
        }

        /* Souris */
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                entrees->souris_presse = true;
                entrees->souris_juste_presse = true;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                entrees->souris_droite_presse = true;
                entrees->souris_droite_juste_presse = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                entrees->souris_presse = false;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                entrees->souris_droite_presse = false;
            }
            break;

        /* Clavier */
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
                entrees->entrees[event.key.keysym.scancode] = true;
                /* N'enregistrer "juste pressé" que pour les vrais appuis, pas les répétitions */
                if (!event.key.repeat) {
                    entrees->entrees_presse[event.key.keysym.scancode] = true;
                }
            }
            break;

        case SDL_KEYUP:
            if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
                entrees->entrees[event.key.keysym.scancode] = false;
            }
            break;

        default:
            break;
        }
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in update input function");
}