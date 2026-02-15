/*
 * Utilitaires Lua et wrapper C++ pour les constantes.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "sol/optional_implementation.hpp"
#include "sol/property.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* bindings_lua_constantes.cpp */
#include <sol/sol.hpp>

extern "C" {
/* On inclut les prototypes des fonctions C (les wrappers Anglais -> Français) */
#include "../../bindings_c/GrnGame.h"
}

/* enregistrement des bindings constantes avec set_function */
void enregistrer_bindings_constantes(sol::table &var) {
    var.set_function("mouseX", &mouseX);
    var.set_function("mouseY", &mouseY);
    var.set_function("mouseLeftJustPressed", &mouseLeftJustPressed);
    var.set_function("mouseRightJustPressed", &mouseRightJustPressed);
    var.set_function("mouseLeftPressed", &mouseLeftPressed);
    var.set_function("mouseRightPressed", &mouseRightPressed);
    var.set_function("mouseScrollVertical", &mouseScrollVertical);
    var.set_function("mouseScrollHorizontal", &mouseScrollHorizontal);
    var.set_function("delta", &delta);
    var.set_function("fps", &fps);
    var.set_function("frameCount", &frameCount);
    var.set_function("offsetX", &offsetX);
    var.set_function("offsetY", &offsetY);
    var.set_function("currentWidth", &currentWidth);
    var.set_function("currentHeight", &currentHeight);
    var.set_function("universeWidth", &universeWidth);
    var.set_function("universeHeight", &universeHeight);
    var.set_function("windowMinimised", &windowMinimised);
}