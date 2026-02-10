/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* verifie si un bouton de manette vient d'etre presse */
bool lua_button_just_pressed(const std::string &button, unsigned char index) {
    return buttonJustPressed(button.c_str(), index);
}

/* verifie si un bouton de manette est maintenu enfonce */
bool lua_button_pressed(const std::string &button, unsigned char index) {
    return buttonPressed(button.c_str(), index);
}

/* initialise une manette a l'index specifie */
void lua_init_controller(unsigned char index) { initController(index); }

/* recuperer les joysticks et convertir en table lua */
sol::table lua_get_joysticks(sol::this_state s, float dead_zone, unsigned char index) {
    sol::state_view lua(s);
    float *joysticks = getJoysticks(dead_zone, index);

    sol::table result = lua.create_table();
    if (joysticks) {
        /* Le tableau contient 6 valeurs: left_x, left_y, right_x, right_y, trigger_left,
         * trigger_right */
        result[1] = joysticks[0];
        result[2] = joysticks[1];
        result[3] = joysticks[2];
        result[4] = joysticks[3];
        result[5] = joysticks[4];
        result[6] = joysticks[5];
    }

    return result;
}

/* ferme une manette a l'index specifie */
void lua_close_controller(unsigned char index) { closeController(index); }

/* ferme un joystick a l'index specifie */
void lua_close_joystick(unsigned char index) { closeJoystick(index); }

/* ferme la manette et le joystick a l'index specifie */
void lua_close_the_controller(unsigned char index) { closeTheController(index); }

/* enregistrement des bindings manette */
void enregistrer_bindings_manette(sol::table &input) {
    input.set_function("buttonJustPressed", &lua_button_just_pressed);
    input.set_function("buttonPressed", &lua_button_pressed);
    input.set_function("initController", &lua_init_controller);
    input.set_function("getJoysticks", &lua_get_joysticks);
    input.set_function("closeController", &lua_close_controller);
    input.set_function("closeJoystick", &lua_close_joystick);
    input.set_function("closeTheController", &lua_close_the_controller);
}