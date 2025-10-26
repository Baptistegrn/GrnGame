#include "input.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL_image.h>
#include <ctype.h>

void input_update(Gestionnaire *jeu, GestionnaireEntrees *entrees)
{
    if (!jeu || !entrees)
    {
        return;
    }

    SDL_Event event;

    entrees->mouse_just_pressed = false;
    entrees->mouse_right_just_pressed = false;
    memset(entrees->keys_pressed, false, sizeof(entrees->keys_pressed));
    memset(entrees->controller_pressed, false, sizeof(entrees->controller_pressed));

    while (SDL_PollEvent(&event))
    {
        int raw_x, raw_y;
        SDL_GetMouseState(&raw_x, &raw_y);

        float coeff_largeur = (jeu->largeur != 0) ? (float)jeu->largeur_actuel / jeu->largeur : 1.0f;
        float coeff_hauteur = (jeu->hauteur != 0) ? (float)jeu->hauteur_actuel / jeu->hauteur : 1.0f;

        entrees->mouse_x = (int)lroundf((raw_x - jeu->decalage_x) / coeff_largeur);
        entrees->mouse_y = (int)lroundf((raw_y - jeu->decalage_y) / coeff_hauteur);

        switch (event.type)
        {
        case SDL_QUIT:
            jeu->run = false;
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            if (event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX)
            {
                entrees->controller[event.cbutton.button] = true;
                entrees->controller_pressed[event.cbutton.button] = true;
            }
            break;

        case SDL_CONTROLLERBUTTONUP:
            if (event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX)
            {
                entrees->controller[event.cbutton.button] = false;
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            switch (event.caxis.axis)
            {
            case SDL_CONTROLLER_AXIS_LEFTX:
                entrees->Joy.left.x = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                entrees->Joy.left.y = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                entrees->Joy.right.x = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                entrees->Joy.right.y = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                entrees->trigger.triggerleft = event.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                entrees->trigger.triggerright = event.caxis.value;
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                entrees->mouse_pressed = true;
                entrees->mouse_just_pressed = true;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                entrees->mouse_right_pressed = true;
                entrees->mouse_right_just_pressed = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                entrees->mouse_pressed = false;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                entrees->mouse_right_pressed = false;
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.scancode < SDL_NUM_SCANCODES)
            {
                entrees->keys[event.key.keysym.scancode] = true;
                entrees->keys_pressed[event.key.keysym.scancode] = true;
            }
            break;

        case SDL_KEYUP:
            if (event.key.keysym.scancode < SDL_NUM_SCANCODES)
            {
                entrees->keys[event.key.keysym.scancode] = false;
            }
            break;

        default:
            break;
        }
    }
}

char *normaliser_nom(const char *src)
{
    if (!src)
    {
        return NULL;
    }

    size_t taille = strlen(src);
    char *dst = malloc(sizeof(char) * (taille + 1));
    if (!dst)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour la normalisation du nom\n");
        return NULL;
    }

    for (size_t i = 0; i < taille; i++)
    {
        dst[i] = tolower(src[i]);
    }
    dst[taille] = '\0';

    return dst;
}

const ToucheNom touches_1[] = {

    {"a", SDL_SCANCODE_A}, {"b", SDL_SCANCODE_B}, {"c", SDL_SCANCODE_C}, {"d", SDL_SCANCODE_D}, {"e", SDL_SCANCODE_E}, {"f", SDL_SCANCODE_F}, {"g", SDL_SCANCODE_G}, {"h", SDL_SCANCODE_H}, {"i", SDL_SCANCODE_I}, {"j", SDL_SCANCODE_J}, {"k", SDL_SCANCODE_K}, {"l", SDL_SCANCODE_L}, {"m", SDL_SCANCODE_M}, {"n", SDL_SCANCODE_N}, {"o", SDL_SCANCODE_O}, {"p", SDL_SCANCODE_P}, {"q", SDL_SCANCODE_Q}, {"r", SDL_SCANCODE_R}, {"s", SDL_SCANCODE_S}, {"t", SDL_SCANCODE_T}, {"u", SDL_SCANCODE_U}, {"v", SDL_SCANCODE_V}, {"w", SDL_SCANCODE_W}, {"x", SDL_SCANCODE_X}, {"y", SDL_SCANCODE_Y}, {"z", SDL_SCANCODE_Z},

    {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},

    {"-", SDL_SCANCODE_MINUS},
    {"=", SDL_SCANCODE_EQUALS},
    {"[", SDL_SCANCODE_LEFTBRACKET},
    {"]", SDL_SCANCODE_RIGHTBRACKET},
    {";", SDL_SCANCODE_SEMICOLON},
    {"'", SDL_SCANCODE_APOSTROPHE},
    {",", SDL_SCANCODE_COMMA},
    {".", SDL_SCANCODE_PERIOD},
    {"/", SDL_SCANCODE_SLASH},
    {"`", SDL_SCANCODE_GRAVE},

    {NULL, SDL_SCANCODE_UNKNOWN}};

const ToucheNom touches_2[] = {

    {"f1", SDL_SCANCODE_F1}, {"f2", SDL_SCANCODE_F2}, {"f3", SDL_SCANCODE_F3}, {"f4", SDL_SCANCODE_F4}, {"f5", SDL_SCANCODE_F5}, {"f6", SDL_SCANCODE_F6}, {"f7", SDL_SCANCODE_F7}, {"f8", SDL_SCANCODE_F8}, {"f9", SDL_SCANCODE_F9},

    {"up", SDL_SCANCODE_UP},
    {"dn", SDL_SCANCODE_DOWN},
    {"lt", SDL_SCANCODE_LEFT},
    {"rt", SDL_SCANCODE_RIGHT},

    {"\\", SDL_SCANCODE_BACKSLASH},

    {NULL, SDL_SCANCODE_UNKNOWN}};

const ToucheNom touches_3[] = {

    {"f10", SDL_SCANCODE_F10}, {"f11", SDL_SCANCODE_F11}, {"f12", SDL_SCANCODE_F12},

    {"esc", SDL_SCANCODE_ESCAPE},
    {"tab", SDL_SCANCODE_TAB},
    {"end", SDL_SCANCODE_END},
    {"alt", SDL_SCANCODE_LALT},

    {"kp0", SDL_SCANCODE_KP_0},
    {"kp1", SDL_SCANCODE_KP_1},
    {"kp2", SDL_SCANCODE_KP_2},
    {"kp3", SDL_SCANCODE_KP_3},
    {"kp4", SDL_SCANCODE_KP_4},
    {"kp5", SDL_SCANCODE_KP_5},
    {"kp6", SDL_SCANCODE_KP_6},
    {"kp7", SDL_SCANCODE_KP_7},
    {"kp8", SDL_SCANCODE_KP_8},
    {"kp9", SDL_SCANCODE_KP_9},
    {"kp+", SDL_SCANCODE_KP_PLUS},
    {"kp-", SDL_SCANCODE_KP_MINUS},
    {"kp*", SDL_SCANCODE_KP_MULTIPLY},
    {"kp/", SDL_SCANCODE_KP_DIVIDE},
    {"kp=", SDL_SCANCODE_KP_EQUALS},
    {"kpe", SDL_SCANCODE_KP_ENTER},
    {"kp.", SDL_SCANCODE_KP_PERIOD},

    {NULL, SDL_SCANCODE_UNKNOWN}};

const ToucheNom touches_longues[] = {

    {"down", SDL_SCANCODE_DOWN}, {"left", SDL_SCANCODE_LEFT}, {"right", SDL_SCANCODE_RIGHT},

    {"home", SDL_SCANCODE_HOME},
    {"pgup", SDL_SCANCODE_PAGEUP},
    {"pgdn", SDL_SCANCODE_PAGEDOWN},
    {"caps", SDL_SCANCODE_CAPSLOCK},
    {"ctrl", SDL_SCANCODE_LCTRL},
    {"menu", SDL_SCANCODE_MENU},
    {"mute", SDL_SCANCODE_AUDIOMUTE},
    {"play", SDL_SCANCODE_AUDIOPLAY},
    {"stop", SDL_SCANCODE_AUDIOSTOP},
    {"next", SDL_SCANCODE_AUDIONEXT},
    {"prev", SDL_SCANCODE_AUDIOPREV},

    {"space", SDL_SCANCODE_SPACE},
    {"enter", SDL_SCANCODE_RETURN},
    {"shift", SDL_SCANCODE_LSHIFT},
    {"pause", SDL_SCANCODE_PAUSE},
    {"break", SDL_SCANCODE_PAUSE},
    {"print", SDL_SCANCODE_PRINTSCREEN},
    {"power", SDL_SCANCODE_POWER},
    {"sleep", SDL_SCANCODE_SLEEP},
    {"lalt", SDL_SCANCODE_LALT},
    {"ralt", SDL_SCANCODE_RALT},
    {"lwin", SDL_SCANCODE_LGUI},
    {"rwin", SDL_SCANCODE_RGUI},
    {"lgui", SDL_SCANCODE_LGUI},
    {"rgui", SDL_SCANCODE_RGUI},
    {"lcmd", SDL_SCANCODE_LGUI},
    {"rcmd", SDL_SCANCODE_RGUI},

    {"return", SDL_SCANCODE_RETURN},
    {"escape", SDL_SCANCODE_ESCAPE},
    {"delete", SDL_SCANCODE_DELETE},
    {"insert", SDL_SCANCODE_INSERT},
    {"lctrl", SDL_SCANCODE_LCTRL},
    {"rctrl", SDL_SCANCODE_RCTRL},
    {"lshift", SDL_SCANCODE_LSHIFT},
    {"rshift", SDL_SCANCODE_RSHIFT},
    {"pageup", SDL_SCANCODE_PAGEUP},
    {"sysreq", SDL_SCANCODE_SYSREQ},
    {"lsuper", SDL_SCANCODE_LGUI},
    {"rsuper", SDL_SCANCODE_RGUI},
    {"volup", SDL_SCANCODE_VOLUMEUP},
    {"prtscr", SDL_SCANCODE_PRINTSCREEN},

    {"numlock", SDL_SCANCODE_NUMLOCKCLEAR},
    {"control", SDL_SCANCODE_LCTRL},
    {"capslock", SDL_SCANCODE_CAPSLOCK},
    {"pagedown", SDL_SCANCODE_PAGEDOWN},
    {"kpenter", SDL_SCANCODE_KP_ENTER},
    {"kpminus", SDL_SCANCODE_KP_MINUS},
    {"kpequals", SDL_SCANCODE_KP_EQUALS},
    {"kpperiod", SDL_SCANCODE_KP_PERIOD},
    {"voldown", SDL_SCANCODE_VOLUMEDOWN},
    {"backspace", SDL_SCANCODE_BACKSPACE},
    {"lcontrol", SDL_SCANCODE_LCTRL},
    {"rcontrol", SDL_SCANCODE_RCTRL},
    {"scrolllock", SDL_SCANCODE_SCROLLLOCK},
    {"kpdivide", SDL_SCANCODE_KP_DIVIDE},
    {"audiodown", SDL_SCANCODE_VOLUMEDOWN},
    {"audiomute", SDL_SCANCODE_AUDIOMUTE},
    {"mediaplay", SDL_SCANCODE_AUDIOPLAY},
    {"mediastop", SDL_SCANCODE_AUDIOSTOP},
    {"medianext", SDL_SCANCODE_AUDIONEXT},
    {"mediaprev", SDL_SCANCODE_AUDIOPREV},
    {"printscreen", SDL_SCANCODE_PRINTSCREEN},
    {"application", SDL_SCANCODE_APPLICATION},
    {"kpmultiply", SDL_SCANCODE_KP_MULTIPLY},
    {"volumedown", SDL_SCANCODE_VOLUMEDOWN},
    {"volumeup", SDL_SCANCODE_VOLUMEUP},
    {"audioup", SDL_SCANCODE_VOLUMEUP},
    {"playpause", SDL_SCANCODE_AUDIOPLAY},
    {"previous", SDL_SCANCODE_AUDIOPREV},
    {"kpplus", SDL_SCANCODE_KP_PLUS},
    {"browserback", SDL_SCANCODE_AC_BACK},
    {"browserfwd", SDL_SCANCODE_AC_FORWARD},
    {"browserstop", SDL_SCANCODE_AC_STOP},
    {"browserhome", SDL_SCANCODE_AC_HOME},
    {"browserrefresh", SDL_SCANCODE_AC_REFRESH},
    {"browsersearch", SDL_SCANCODE_AC_SEARCH},

    {NULL, SDL_SCANCODE_UNKNOWN}};

const ManetteBoutonNom boutons_manette[] = {
    {"a", SDL_CONTROLLER_BUTTON_A}, {"b", SDL_CONTROLLER_BUTTON_B}, {"x", SDL_CONTROLLER_BUTTON_X}, {"y", SDL_CONTROLLER_BUTTON_Y}, {"start", SDL_CONTROLLER_BUTTON_START}, {"back", SDL_CONTROLLER_BUTTON_BACK}, {"select", SDL_CONTROLLER_BUTTON_BACK}, {"guide", SDL_CONTROLLER_BUTTON_GUIDE}, {"home", SDL_CONTROLLER_BUTTON_GUIDE}, {"lb", SDL_CONTROLLER_BUTTON_LEFTSHOULDER}, {"rb", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER}, {"l1", SDL_CONTROLLER_BUTTON_LEFTSHOULDER}, {"r1", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER}, {"l3", SDL_CONTROLLER_BUTTON_LEFTSTICK}, {"r3", SDL_CONTROLLER_BUTTON_RIGHTSTICK}, {"up", SDL_CONTROLLER_BUTTON_DPAD_UP}, {"down", SDL_CONTROLLER_BUTTON_DPAD_DOWN}, {"left", SDL_CONTROLLER_BUTTON_DPAD_LEFT}, {"right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT}, {"share", SDL_CONTROLLER_BUTTON_MISC1}, {"paddle1", SDL_CONTROLLER_BUTTON_PADDLE1}, {"paddle2", SDL_CONTROLLER_BUTTON_PADDLE2}, {"paddle3", SDL_CONTROLLER_BUTTON_PADDLE3}, {"paddle4", SDL_CONTROLLER_BUTTON_PADDLE4}, {"touchpad", SDL_CONTROLLER_BUTTON_TOUCHPAD}, {NULL, SDL_CONTROLLER_BUTTON_INVALID}};

 SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise)
{
    char *nom = normaliser_nom(nom_non_normalise);
    if (!nom || strlen(nom) == 0)
    {
        free(nom);
        fprintf(stderr, "Erreur: Nom de touche NULL ou vide\n");
        return SDL_SCANCODE_UNKNOWN;
    }

    const ToucheNom *liste = NULL;
    size_t longueur = strlen(nom);

    if (longueur == 1)
    {
        liste = touches_1;
    }
    else if (longueur == 2)
    {
        liste = touches_2;
    }
    else if (longueur == 3)
    {
        liste = touches_3;
    }
    else
    {
        liste = touches_longues;
    }

    for (int i = 0; liste[i].nom; i++)
    {
        if (strcmp(nom, liste[i].nom) == 0)
        {
            free(nom);
            return liste[i].code;
        }
    }

    fprintf(stderr, "Erreur: Nom de touche inconnu '%s'\n", nom);
    free(nom);
    return SDL_SCANCODE_UNKNOWN;
}

 SDL_GameControllerButton bouton_manette_depuis_nom(const char *nom_non_normalise)
{
    char *nom = normaliser_nom(nom_non_normalise);
    if (!nom || strlen(nom) == 0)
    {
        free(nom);
        fprintf(stderr, "Erreur: Nom de bouton manette NULL ou vide\n");
        return SDL_CONTROLLER_BUTTON_INVALID;
    }

    for (int i = 0; boutons_manette[i].nom; i++)
    {
        if (strcmp(nom, boutons_manette[i].nom) == 0)
        {
            free(nom);
            return boutons_manette[i].bouton;
        }
    }

    fprintf(stderr, "Erreur: Nom de bouton manette inconnu '%s'\n", nom);
    free(nom);
    return SDL_CONTROLLER_BUTTON_INVALID;
}

bool touche_juste_presse(Gestionnaire *jeu, const char *touche)
{
    if (!jeu || !jeu->entrees || !touche)
    {
        return false;
    }

    SDL_Scancode sc = scancode_depuis_nom(touche);
    return (sc != SDL_SCANCODE_UNKNOWN) && jeu->entrees->keys_pressed[sc];
}

bool touche_enfoncee(Gestionnaire *jeu, const char *touche)
{
    if (!jeu || !jeu->entrees || !touche)
    {
        return false;
    }

    SDL_Scancode sc = scancode_depuis_nom(touche);
    return (sc != SDL_SCANCODE_UNKNOWN) && jeu->entrees->keys[sc];
}

bool touche_mannette_juste_presse(Gestionnaire *jeu, const char *touche)
{
    if (!jeu || !jeu->entrees || !touche)
    {
        return false;
    }

    SDL_GameControllerButton bt = bouton_manette_depuis_nom(touche);
    return (bt != SDL_CONTROLLER_BUTTON_INVALID) && jeu->entrees->controller_pressed[bt];
}

bool touche_mannette_enfoncee(Gestionnaire *jeu, const char *touche)
{
    if (!jeu || !jeu->entrees || !touche)
    {
        return false;
    }

    SDL_GameControllerButton bt = bouton_manette_depuis_nom(touche);
    return (bt != SDL_CONTROLLER_BUTTON_INVALID) && jeu->entrees->controller[bt];
}