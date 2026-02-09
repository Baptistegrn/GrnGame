/*
 * Utilitaires Lua et wrapper C++ pour les constantes.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* enregistrer les bindings de constantes */
void enregistrer_bindings_constantes(sol::table &var) {
    var["mouseX"] = sol::property(&mouseX);
    var["mouseY"] = sol::property(&mouseY);
    var["mouseLeftJustPressed"] = sol::property(&mouseLeftJustPressed);
    var["mouseRightJustPressed"] = sol::property(&mouseRightJustPressed);
    var["mouseLeftPressed"] = sol::property(&mouseLeftPressed);
    var["mouseRightPressed"] = sol::property(&mouseRightPressed);
    var["mouseScrollVertical"] = sol::property(&mouseScrollVertical);
    var["mouseScrollHorizontal"] = sol::property(&mouseScrollHorizontal);
    var["delta"] = sol::property(&delta);
    var["fps"] = sol::property(&fps);
    var["frameCount"] = sol::property(&frameCount);
    var["offsetX"] = sol::property(&offsetX);
    var["offsetY"] = sol::property(&offsetY);
    var["currentWidth"] = sol::property(&currentWidth);
    var["currentHeight"] = sol::property(&currentHeight);
    var["universeWidth"] = sol::property(&universeWidth);
    var["universeHeight"] = sol::property(&universeHeight);
    var["windowMinimised"] = sol::property(&windowMinimised);
}