/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/entrees/entrees.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* verifie si un bouton de manette vient d'etre presse */
bool lua_boutton_juste_presse(const std::string &button, unsigned char index) {
    return touche_manette_juste_presse(button.c_str(), index);
}

/* verifie si un bouton de manette est maintenu enfonce */
bool lua_boutton_presse(const std::string &button, unsigned char index) {
    return touche_manette_enfoncee(button.c_str(), index);
}

/* initialise une manette a l'index specifie */
void lua_init_controller(unsigned char index) { init_controller_joysticks(index); }

/* recuperer les joysticks et convertir en table lua */
sol::table lua_renvoie_joysticks(sol::this_state s, float dead_zone, unsigned char index) {
    sol::state_view lua(s);
    float *joysticks = renvoie_joysticks(dead_zone, index);

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
void lua_fermer_controller(unsigned char index) { fermer_controller(index); }

/* ferme un joystick a l'index specifie */
void lua_fermer_joystick(unsigned char index) { fermer_joystick(index); }

/* ferme la manette et le joystick a l'index specifie */
void lua_fermer_le_controller(unsigned char index) {
    fermer_controller(index);
    fermer_joystick(index);
}

/* enregistrement des bindings manette */
void enregistrer_bindings_manette(sol::table &input) {
    input.set_function("buttonJustPressed", &lua_boutton_juste_presse);
    input.set_function("buttonPressed", &lua_boutton_presse);
    input.set_function("initController", &lua_init_controller);
    input.set_function("getJoysticks", &lua_renvoie_joysticks);
    input.set_function("closeController", &lua_fermer_controller);
    input.set_function("closeJoystick", &lua_fermer_joystick);
    input.set_function("closeTheController", &lua_fermer_le_controller);
}