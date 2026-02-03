/*
 * Mise a jour des entrees utilisateur (clavier, souris, manette).
 * Traite la file d'evenements SDL a chaque frame.
 */

#include "../../main.h"
#include <math.h>
#include <string.h>

/*
 * Trouve l'index local (0 a NB_MANETTES_MAX-1) d'une manette a partir de
 * son instance ID SDL. Retourne -1 si non trouvee.
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
 * Met a jour les entrees (Clavier, Souris, Manette) pour la frame actuelle.
 * Reinitialise les etats "juste presse", recupere la position souris corrigee
 * en tenant compte du letterboxing, puis traite la file d'evenements SDL.
 */
void mise_a_jour_input() {
    if (!gs)
        goto gsvide;
    GestionnaireEntrees *entrees = gs->entrees;
    GestionnaireFenetre *fenetre = gs->fenetre;
    SDL_Event event;

    /* Reinitialisation des etats "Just Pressed" (duree 1 frame) */
    entrees->souris_juste_presse = false;
    entrees->souris_droite_juste_presse = false;
    entrees->souris_scroll_y = 0;
    entrees->souris_scroll_x = 0;

    memset(entrees->entrees_presse, false, sizeof(entrees->entrees_presse));
    for (int i = 0; i < NB_MANETTES_MAX; i++) {
        for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; j++) {
            entrees->manette_presse[j][i] = false;
        }
    }

    /* Traitement de la file d'evenements SDL */
    while (SDL_PollEvent(&event)) {

        /* Mise a jour de la position souris avec correction du letterboxing.
           Les coordonnees ecran (physiques) sont converties en coordonnees jeu
           (logiques) en tenant compte du ratio et des bandes noires. */
        int raw_x, raw_y;
        SDL_GetMouseState(&raw_x, &raw_y);

        int coeff = fenetre->coeff;

        entrees->souris_x =
            (int)lroundf(((float)raw_x - (float)fenetre->decalage_x) / (float)coeff);

        entrees->souris_y =
            (int)lroundf(((float)raw_y - (float)fenetre->decalage_y) / (float)coeff);

        /* Dispatch des evenements */
        switch (event.type) {
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                entrees->souris_scroll_y = 1;
            } else if (event.wheel.y < 0) {
                entrees->souris_scroll_y = -1;
            }
            if (event.wheel.x > 0) {
                entrees->souris_scroll_x = 1;
            } else if (event.wheel.x < 0) {
                entrees->souris_scroll_x = -1;
            }
            break;
        case SDL_QUIT:
            gs->timing->run = false;
            break;
        /* Tactile -> map sur souris */
        case SDL_FINGERDOWN: {
            int px = lroundf(event.tfinger.x * fenetre->largeur_univers) +
                     lroundf((float)fenetre->decalage_x / fenetre->coeff);
            int py = lroundf(event.tfinger.y * fenetre->hauteur_univers) +
                     lroundf((float)fenetre->decalage_y / fenetre->coeff);

            entrees->souris_x = px;
            entrees->souris_y = py;

            entrees->souris_presse = true;
            entrees->souris_juste_presse = true;
            break;
        }

        case SDL_FINGERUP:
            entrees->souris_presse = false;
            break;

        case SDL_FINGERMOTION: {
            int px = lroundf(event.tfinger.x * fenetre->largeur_univers) +
                     lroundf((float)fenetre->decalage_x / fenetre->coeff);
            int py = lroundf(event.tfinger.y * fenetre->hauteur_univers) +
                     lroundf((float)fenetre->decalage_y / fenetre->coeff);
            entrees->souris_x = px;
            entrees->souris_y = py;
            break;
        }
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

        /* Manette : Connexion/Deconnexion */
        case SDL_CONTROLLERDEVICEADDED:
            init_controller_joysticks(event.cdevice.which);
            break;
        case SDL_CONTROLLERDEVICEREMOVED: {
            int idx = trouver_index_manette(event.cdevice.which);
            if (idx >= 0) {
                fermer_controller(idx);
                fermer_joystick(idx);
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
                /* N'enregistrer "juste presse" que pour les vrais appuis, pas les repetitions */
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