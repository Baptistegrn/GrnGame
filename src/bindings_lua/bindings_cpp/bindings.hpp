/*
 * Wrapper C++ pour les bindings Lua.
 */

#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#ifdef __cplusplus

#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* enregistre les bindings de module jeu  */
void enregistrer_bindings_module_jeu(sol::table &game);

/* enregistre les bindings de image */
void enregistrer_bindings_image(sol::table &image);

/* enregistre les bindings de son */
void enregistrer_bindings_son(sol::table &song);

/* enregistre les bindings de utils */
void enregistrer_bindings_utils(sol::table &utils);

/* enregistre les bindings de clavier */
void enregistrer_bindings_clavier(sol::table &input);

/* enregistre les bindings de manette */
void enregistrer_bindings_manette(sol::table &input);

/* enregistre les bindings de constantes */
void enregistrer_bindings_constantes(sol::table &var);

#endif /* __cplusplus */

#endif /* BINDINGS_HPP */