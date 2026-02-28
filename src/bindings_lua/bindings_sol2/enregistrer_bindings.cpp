/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "bindings.hpp"
#include <sol/sol.hpp>

extern "C" {
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
    sol::table json = lua_view.create_named_table("json");
    sol::table mouse = lua_view.create_named_table("mouse");
    sol::table window = lua_view.create_named_table("window");
    sol::table camera = lua_view.create_named_table("camera");
    enregistrer_bindings_utils(utils);
    enregistrer_bindings_son(song);
    enregistrer_bindings_image(image);
    enregistrer_bindings_camera(camera);
    enregistrer_bindings_clavier(input);
    enregistrer_bindings_manette(input);
    enregistrer_bindings_constantes(var);
    enregistrer_bindings_json(json);
    enregistrer_bindings_fenetre(window);
    enregistrer_bindings_souris(mouse);
}
