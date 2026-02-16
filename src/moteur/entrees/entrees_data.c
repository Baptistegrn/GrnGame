/*
 * Tables de correspondance entre noms de touches et codes SDL.
 * Permet la conversion dynamique d'une chaine (ex: "space") vers le scancode.
 */

#include "entrees.h"

/* Touches alphabetiques, numeriques et symboles */
const ToucheNom touches_1[] = {{"a", SDL_SCANCODE_A},           {"b", SDL_SCANCODE_B},
                               {"c", SDL_SCANCODE_C},           {"d", SDL_SCANCODE_D},
                               {"e", SDL_SCANCODE_E},           {"f", SDL_SCANCODE_F},
                               {"g", SDL_SCANCODE_G},           {"h", SDL_SCANCODE_H},
                               {"i", SDL_SCANCODE_I},           {"j", SDL_SCANCODE_J},
                               {"k", SDL_SCANCODE_K},           {"l", SDL_SCANCODE_L},
                               {"m", SDL_SCANCODE_M},           {"n", SDL_SCANCODE_N},
                               {"o", SDL_SCANCODE_O},           {"p", SDL_SCANCODE_P},
                               {"q", SDL_SCANCODE_Q},           {"r", SDL_SCANCODE_R},
                               {"s", SDL_SCANCODE_S},           {"t", SDL_SCANCODE_T},
                               {"u", SDL_SCANCODE_U},           {"v", SDL_SCANCODE_V},
                               {"w", SDL_SCANCODE_W},           {"x", SDL_SCANCODE_X},
                               {"y", SDL_SCANCODE_Y},           {"z", SDL_SCANCODE_Z},

                               {"0", SDL_SCANCODE_0},           {"1", SDL_SCANCODE_1},
                               {"2", SDL_SCANCODE_2},           {"3", SDL_SCANCODE_3},
                               {"4", SDL_SCANCODE_4},           {"5", SDL_SCANCODE_5},
                               {"6", SDL_SCANCODE_6},           {"7", SDL_SCANCODE_7},
                               {"8", SDL_SCANCODE_8},           {"9", SDL_SCANCODE_9},

                               {"-", SDL_SCANCODE_MINUS},       {"=", SDL_SCANCODE_EQUALS},
                               {"[", SDL_SCANCODE_LEFTBRACKET}, {"]", SDL_SCANCODE_RIGHTBRACKET},
                               {";", SDL_SCANCODE_SEMICOLON},   {"'", SDL_SCANCODE_APOSTROPHE},
                               {",", SDL_SCANCODE_COMMA},       {".", SDL_SCANCODE_PERIOD},
                               {"/", SDL_SCANCODE_SLASH},       {"`", SDL_SCANCODE_GRAVE},
                               {"\\", SDL_SCANCODE_BACKSLASH},

                               {NULL, SDL_SCANCODE_UNKNOWN}};

/* Touches fonctions et fleches */
const ToucheNom touches_2[] = {
    {"f1", SDL_SCANCODE_F1},     {"f2", SDL_SCANCODE_F2},   {"f3", SDL_SCANCODE_F3},
    {"f4", SDL_SCANCODE_F4},     {"f5", SDL_SCANCODE_F5},   {"f6", SDL_SCANCODE_F6},
    {"f7", SDL_SCANCODE_F7},     {"f8", SDL_SCANCODE_F8},   {"f9", SDL_SCANCODE_F9},

    {"up", SDL_SCANCODE_UP},     {"dn", SDL_SCANCODE_DOWN}, {"lt", SDL_SCANCODE_LEFT},
    {"rt", SDL_SCANCODE_RIGHT},

    {NULL, SDL_SCANCODE_UNKNOWN}};

/* Touches F10-F12 et pave numerique */
const ToucheNom touches_3[] = {{"f10", SDL_SCANCODE_F10},         {"f11", SDL_SCANCODE_F11},
                               {"f12", SDL_SCANCODE_F12},

                               {"esc", SDL_SCANCODE_ESCAPE},      {"tab", SDL_SCANCODE_TAB},
                               {"end", SDL_SCANCODE_END},         {"alt", SDL_SCANCODE_LALT},

                               {"kp0", SDL_SCANCODE_KP_0},        {"kp1", SDL_SCANCODE_KP_1},
                               {"kp2", SDL_SCANCODE_KP_2},        {"kp3", SDL_SCANCODE_KP_3},
                               {"kp4", SDL_SCANCODE_KP_4},        {"kp5", SDL_SCANCODE_KP_5},
                               {"kp6", SDL_SCANCODE_KP_6},        {"kp7", SDL_SCANCODE_KP_7},
                               {"kp8", SDL_SCANCODE_KP_8},        {"kp9", SDL_SCANCODE_KP_9},

                               {"kp+", SDL_SCANCODE_KP_PLUS},     {"kp-", SDL_SCANCODE_KP_MINUS},
                               {"kp*", SDL_SCANCODE_KP_MULTIPLY}, {"kp/", SDL_SCANCODE_KP_DIVIDE},
                               {"kp=", SDL_SCANCODE_KP_EQUALS},   {"kpe", SDL_SCANCODE_KP_ENTER},
                               {"kp.", SDL_SCANCODE_KP_PERIOD},

                               {NULL, SDL_SCANCODE_UNKNOWN}};

/* Touches longues et alias */
const ToucheNom touches_longues[] = {{"space", SDL_SCANCODE_SPACE},
                                     {"enter", SDL_SCANCODE_RETURN},
                                     {"return", SDL_SCANCODE_RETURN},
                                     {"escape", SDL_SCANCODE_ESCAPE},
                                     {"backspace", SDL_SCANCODE_BACKSPACE},
                                     {"delete", SDL_SCANCODE_DELETE},
                                     {"insert", SDL_SCANCODE_INSERT},

                                     {"shift", SDL_SCANCODE_LSHIFT},
                                     {"lshift", SDL_SCANCODE_LSHIFT},
                                     {"rshift", SDL_SCANCODE_RSHIFT},

                                     {"ctrl", SDL_SCANCODE_LCTRL},
                                     {"control", SDL_SCANCODE_LCTRL},
                                     {"lctrl", SDL_SCANCODE_LCTRL},
                                     {"rctrl", SDL_SCANCODE_RCTRL},

                                     {"alt", SDL_SCANCODE_LALT},
                                     {"lalt", SDL_SCANCODE_LALT},
                                     {"ralt", SDL_SCANCODE_RALT},

                                     {"caps", SDL_SCANCODE_CAPSLOCK},
                                     {"capslock", SDL_SCANCODE_CAPSLOCK},
                                     {"numlock", SDL_SCANCODE_NUMLOCKCLEAR},
                                     {"scrolllock", SDL_SCANCODE_SCROLLLOCK},

                                     {"home", SDL_SCANCODE_HOME},
                                     {"end", SDL_SCANCODE_END},
                                     {"pageup", SDL_SCANCODE_PAGEUP},
                                     {"pagedown", SDL_SCANCODE_PAGEDOWN},

                                     {"print", SDL_SCANCODE_PRINTSCREEN},
                                     {"printscreen", SDL_SCANCODE_PRINTSCREEN},
                                     {"prtscr", SDL_SCANCODE_PRINTSCREEN},

                                     {"pause", SDL_SCANCODE_PAUSE},
                                     {"break", SDL_SCANCODE_PAUSE},

                                     {"volumeup", SDL_SCANCODE_VOLUMEUP},
                                     {"volup", SDL_SCANCODE_VOLUMEUP},
                                     {"voldown", SDL_SCANCODE_VOLUMEDOWN},
                                     {"volumedown", SDL_SCANCODE_VOLUMEDOWN},
                                     {"mute", SDL_SCANCODE_AUDIOMUTE},

                                     {"play", SDL_SCANCODE_AUDIOPLAY},
                                     {"stop", SDL_SCANCODE_AUDIOSTOP},
                                     {"next", SDL_SCANCODE_AUDIONEXT},
                                     {"prev", SDL_SCANCODE_AUDIOPREV},

                                     {NULL, SDL_SCANCODE_UNKNOWN}};

/* Correspondance des boutons de manette */
const ManetteBoutonNom boutons_manette[] = {{"a", SDL_CONTROLLER_BUTTON_A},
                                            {"b", SDL_CONTROLLER_BUTTON_B},
                                            {"x", SDL_CONTROLLER_BUTTON_X},
                                            {"y", SDL_CONTROLLER_BUTTON_Y},

                                            {"start", SDL_CONTROLLER_BUTTON_START},
                                            {"back", SDL_CONTROLLER_BUTTON_BACK},
                                            {"select", SDL_CONTROLLER_BUTTON_BACK},
                                            {"guide", SDL_CONTROLLER_BUTTON_GUIDE},
                                            {"home", SDL_CONTROLLER_BUTTON_GUIDE},

                                            {"lb", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
                                            {"rb", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
                                            {"l1", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
                                            {"r1", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},

                                            {"l3", SDL_CONTROLLER_BUTTON_LEFTSTICK},
                                            {"r3", SDL_CONTROLLER_BUTTON_RIGHTSTICK},

                                            {"up", SDL_CONTROLLER_BUTTON_DPAD_UP},
                                            {"down", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
                                            {"left", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
                                            {"right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},

                                            {"share", SDL_CONTROLLER_BUTTON_MISC1},
                                            {"touchpad", SDL_CONTROLLER_BUTTON_TOUCHPAD},

                                            {"paddle1", SDL_CONTROLLER_BUTTON_PADDLE1},
                                            {"paddle2", SDL_CONTROLLER_BUTTON_PADDLE2},
                                            {"paddle3", SDL_CONTROLLER_BUTTON_PADDLE3},
                                            {"paddle4", SDL_CONTROLLER_BUTTON_PADDLE4},

                                            {NULL, SDL_CONTROLLER_BUTTON_INVALID}};
