/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/entrees/entrees.h"
#include "../../moteur/fenetre/fenetre.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* verifie si une touche vient d'etre pressee */
bool lua_touche_juste_presse(const std::string &key) { return touche_juste_presse(key.c_str()); }

/* verifie si une touche est maintenue enfoncee */
bool lua_touche_enfoncee(const std::string &key) { return touche_enfoncee(key.c_str()); }

/* enregistrement des bindings clavier */
void enregistrer_bindings_clavier(sol::table &input) {
    input.set_function("keyJustPressed", &lua_touche_juste_presse);
    input.set_function("keyPressed", &lua_touche_enfoncee);
    input.set_function("showCursor", &afficher_curseur);
}