/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "bindings.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

extern "C" void enregistrer_bindings(void *L) {
    lua_State *lua = static_cast<lua_State *>(L);

    sol::state_view lua_view(lua);

    sol::table utils = lua_view.create_named_table("utils");
    sol::table song = lua_view.create_named_table("song");
    sol::table game = lua_view.create_named_table("game");
    sol::table image = lua_view.create_named_table("image");
    sol::table input = lua_view.create_named_table("input");
    sol::table var = lua_view.create_named_table("var");
    enregistrer_bindings_utils(utils);
    enregistrer_bindings_son(song);
    enregistrer_bindings_image(image);
    enregistrer_bindings_module_jeu(game);
    enregistrer_bindings_clavier(input);
    enregistrer_bindings_manette(input);
    enregistrer_bindings_constantes(var);
}
