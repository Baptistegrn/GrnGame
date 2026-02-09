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

/* verfie si une touche viens detre presse */
bool lua_key_just_pressed(const std::string &key) { return keyJustPressed(key.c_str()); }

/* verfie si une touche est maintenue enfonce */
bool lua_key_pressed(const std::string &key) { return keyPressed(key.c_str()); }

/* enregistrer les bindings de clavier */
void enregistrer_bindings_clavier(sol::table &input) {
    input.set_function("keyJustPressed", &lua_key_just_pressed);
    input.set_function("keyPressed", &lua_key_pressed);
    input.set_function("showCursor", &showCursor);
}