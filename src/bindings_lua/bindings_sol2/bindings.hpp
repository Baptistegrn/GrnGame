/*
 * Wrapper C++ pour les bindings Lua.
 */

#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#ifdef __cplusplus

#include <sol/sol.hpp>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

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

/* enregistre les bindings de json */
void enregistrer_bindings_json(sol::table &json);

/*enregistre les bindings de fenetre */
void enregistrer_bindings_fenetre(sol::table &window);

/*enregistre les bindings de souris */
void enregistrer_bindings_souris(sol::table &mouse);

/* enregistre les bindings de camera */
void enregistrer_bindings_camera(sol::table &camera);

/* enregistre les bindings de entite */
void enregistrer_bindings_entite(sol::table &entite);

/* enregistrement des bindings blocs */
void enregistrer_bindings_blocs(sol::table &blocks);

#endif /* __cplusplus */

#endif /* BINDINGS_HPP */