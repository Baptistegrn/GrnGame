/*
 * Mise a jour des entrees (clavier, souris, manette).
 * Traite la file d'evenements SDL a chaque frame.
 */

#include "../../main.h"
#include "../boucle/boucle.h"
#include "../fenetre/fenetre.h"
#include "../logging/logging.h"
#include "entrees.h"
#include <string.h>

/* Trouve l'index local d'une manette a partir de son instance ID SDL */
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

/* Met a jour les entrees pour la frame actuelle */
void mise_a_jour_input() {
    if (!gs)
        goto gsvide;

    GestionnaireEntrees *entrees = gs->entrees;
    GestionnaireFenetre *fenetre = gs->fenetre;
    SDL_Event event;

    /* Reinitialisation des etats "juste presse" */
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

    /* Mise a jour position souris */
    int souris_x, souris_y;
    SDL_GetMouseState(&souris_x, &souris_y);
    int coeff = fenetre->coeff;
    entrees->souris_x = (int)lroundf(((float)souris_x - (float)fenetre->decalage_x) / (float)coeff);
    entrees->souris_y = (int)lroundf(((float)souris_y - (float)fenetre->decalage_y) / (float)coeff);

    /* Traitement des evenements SDL */
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_TEXTINPUT:
            copie_texte_saisie(event.text.text);
            break;
        case SDL_DROPFILE:
            strncpy(entrees->fichier_drop, event.drop.file, sizeof(entrees->fichier_drop) - 1);
            entrees->fichier_drop[sizeof(entrees->fichier_drop) - 1] = '\0';
            SDL_free(event.drop.file);
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                gs->timing->run = false;
                break;
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                appliquer_redimensionnement(event.window.data1, event.window.data2);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED: {
                /* ​recuperation de la taille obligatoire pour le plein ecran fenetre ​ */
                int w, h;
                SDL_GetWindowSize(gs->fenetre->fenetre, &w, &h);

                appliquer_redimensionnement(w, h);
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_LOST:
                gs->timing->en_pause = true;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                gs->timing->en_pause = true;
                gs->timing->minimise = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                gs->timing->en_pause = false;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                gs->timing->en_pause = false;
                gs->timing->minimise = false;
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0)
                entrees->souris_scroll_y = 1;
            else if (event.wheel.y < 0)
                entrees->souris_scroll_y = -1;
            if (event.wheel.x > 0)
                entrees->souris_scroll_x = 1;
            else if (event.wheel.x < 0)
                entrees->souris_scroll_x = -1;
            break;

        case SDL_QUIT:
            gs->timing->run = false;
            break;

        case SDL_FINGERDOWN: {
            entrees->souris_x = lroundf(event.tfinger.x * fenetre->largeur_univers) +
                                lroundf((float)fenetre->decalage_x / fenetre->coeff);
            entrees->souris_y = lroundf(event.tfinger.y * fenetre->hauteur_univers) +
                                lroundf((float)fenetre->decalage_y / fenetre->coeff);
            entrees->souris_presse = true;
            entrees->souris_juste_presse = true;
            break;
        }

        case SDL_FINGERUP:
            entrees->souris_presse = false;
            break;

        case SDL_FINGERMOTION: {
            entrees->souris_x = lroundf(event.tfinger.x * fenetre->largeur_univers) +
                                lroundf((float)fenetre->decalage_x / fenetre->coeff);
            entrees->souris_y = lroundf(event.tfinger.y * fenetre->hauteur_univers) +
                                lroundf((float)fenetre->decalage_y / fenetre->coeff);
            break;
        }

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
            if (event.button.button == SDL_BUTTON_LEFT)
                entrees->souris_presse = false;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                entrees->souris_droite_presse = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
                entrees->entrees[event.key.keysym.scancode] = true;
                if (!event.key.repeat)
                    entrees->entrees_presse[event.key.keysym.scancode] = true;
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