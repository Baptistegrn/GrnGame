/*
 * Utilitaires Lua et wrapper C++ pour les constantes.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include <sol/sol.hpp>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* bindings_lua_constantes.cpp */
#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/constante/constante.h"
}

/* enregistrement des bindings constantes avec set_function */
void enregistrer_bindings_constantes(sol::table &var) {
    var.set_function("mouseX", &souris_x);
    var.set_function("mouseY", &souris_y);
    var.set_function("mouseLeftJustPressed", &souris_gauche_juste_presse);
    var.set_function("mouseRightJustPressed", &souris_droite_juste_presse);
    var.set_function("mouseLeftPressed", &souris_gauche_presse);
    var.set_function("mouseRightPressed", &souris_droite_presse);
    var.set_function("mouseScrollVertical", &souris_scroll_vertical);
    var.set_function("mouseScrollHorizontal", &souris_scroll_horizontal);
    var.set_function("delta", &dt);
    var.set_function("fps", &nb_images);
    var.set_function("frameCount", &compteur_frame);
    var.set_function("offsetX", &decalage_x);
    var.set_function("offsetY", &decalage_y);
    var.set_function("currentWidth", &largeur_actuelle);
    var.set_function("currentHeight", &hauteur_actuelle);
    var.set_function("universeWidth", &largeur_univers);
    var.set_function("universeHeight", &hauteur_univers);
    var.set_function("windowMinimised", &fenetre_est_minimise);
    var.set_function("getTextDrop", &recuperer_drop_file);
}