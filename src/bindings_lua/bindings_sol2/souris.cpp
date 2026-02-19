/* enregistre les bindings de souris dans la vm lua */

#include <sol/sol.hpp>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/constante/constante.h"
#include "../../moteur/fenetre/fenetre.h"
}

/* enregistrement des bindings souris */
void enregistrer_bindings_souris(sol::table &mouse) {
    mouse.set_function("X", &souris_x);
    mouse.set_function("Y", &souris_y);
    mouse.set_function("LeftJustPressed", &souris_gauche_juste_presse);
    mouse.set_function("RightJustPressed", &souris_droite_juste_presse);
    mouse.set_function("LeftPressed", &souris_gauche_presse);
    mouse.set_function("RightPressed", &souris_droite_presse);
    mouse.set_function("ScrollVertical", &souris_scroll_vertical);
    mouse.set_function("ScrollHorizontal", &souris_scroll_horizontal);
    mouse.set_function("showCursor", &afficher_curseur);
}