/*
 * Rechargement a chaud des scripts Lua.
 * Permet de modifier les scripts sans redemarrer le jeu (mode debug).
 */

#include "../main.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

extern lua_State *G_LuaState;

/*
 * Recharge un module Lua a partir de son chemin.
 * Ne peut pas recharger le point d'entree (main.lua).
 * Invalide le cache dans package.loaded avant de recharger.
 */
void recharger_lua(const char *chemin) {
    if (!G_LuaState)
        return;

    char dst[TAILLE_LIEN];
    normaliser_chemin_copies(dst, chemin);

    const char *nom_module = recupere_module(dst);
    if (!nom_module) {
        log_fmt(NiveauLogErreur, "Impossible to retrieve module name from path: %s", chemin);
        return;
    }

    /* Le main.lua ne peut pas etre recharge a chaud */
    if (strcmp(nom_module, "main") == 0) {
        log_message(NiveauLogInfo, "main.lua is modifing hot reload skiped...");
        free_gestion_echec_compteur((char *)nom_module);
        return;
    }

    log_fmt(NiveauLogDebug, "recharging of module : %s", nom_module);

    /* Invalidation du module dans package.loaded */
    lua_getglobal(G_LuaState, "package");
    lua_getfield(G_LuaState, -1, "loaded");
    lua_pushnil(G_LuaState);
    lua_setfield(G_LuaState, -2, nom_module);
    lua_pop(G_LuaState, 2);

    /* Chargement du fichier */
    int status = luaL_loadfile(G_LuaState, chemin);
    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Syntax error in %s: %s", chemin, lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
        free_gestion_echec_compteur((char *)nom_module);
        return;
    }

    /* Execution du module */
    status = lua_pcall(G_LuaState, 0, 1, 0);
    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Runtime error in %s: %s", chemin, lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
        free_gestion_echec_compteur((char *)nom_module);
        return;
    }

    /* Mise a jour de package.loaded avec le nouveau module */
    lua_getglobal(G_LuaState, "package");
    lua_getfield(G_LuaState, -1, "loaded");
    lua_pushvalue(G_LuaState, -3);
    lua_setfield(G_LuaState, -2, nom_module);
    lua_pop(G_LuaState, 3);

    log_fmt(NiveauLogInfo, "Module '%s' reloaded", nom_module);
    free_gestion_echec_compteur((char *)nom_module);
}

/*
 * Verifie periodiquement les fichiers Lua modifies.
 * Appele a chaque frame, declenche le rechargement selon SECONDES_RELOAD_LUA.
 */
void actualiser_rechargement(void) {
    Uint32 frame = gs->timing->compteur_frames;
    float fps = gs->timing->fps;
    Fichiers *a_modifier = NULL;

    if (frame % (SECONDES_RELOAD_LUA * (int)fps) == 0 && frame != 0) {
        /* Liberation de l'ancienne liste */
        if (gs->frame->fichiers_lua) {
            liberer_fichiers(gs->frame->fichiers_lua);
            gs->frame->fichiers_lua = NULL;
        }

        /* Scan des fichiers Lua */
        gs->frame->fichiers_lua = renvoie_fichier_dossier("../src", "lua", NULL);

        /* Rechargement des fichiers modifies */
        a_modifier = renvoie_fichier_modifie(gs->frame->fichiers_lua);
        for (int i = 0; i < a_modifier->taille; i++) {
            log_fmt(NiveauLogInfo, "File  %s is modified ,reload it", a_modifier->noms[i]);
            recharger_lua(a_modifier->noms[i]);
        }
        liberer_fichiers(a_modifier);
    }
}