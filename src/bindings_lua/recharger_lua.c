/*
 * Rechargement a chaud des scripts Lua (hot reload).
 * Permet de modifier et recharger les scripts sans redemarrer le jeu en mode debug.
 * Le point d'entree (main.lua) ne peut pas etre recharge.
 */

#include "../allouer/allouer.h"
#include "../chemin/chemin.h"
#include "../main.h"
#include "../moteur/boucle/boucle.h"
#include "../moteur/logging/logging.h"
#include "SDL_stdinc.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <string.h>


extern lua_State *G_LuaState;

/*
 * Recharge un module Lua a partir de son chemin.
 * Invalide le cache dans package.loaded avant de recharger.
 * main.lua ne peut pas etre recharge (point d'entree).
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
        log_message(NiveauLogInfo, "main.lua modification: hot reload skipped");
        free_gestion_echec_compteur((char *)nom_module);
        return;
    }

    log_fmt(NiveauLogDebug, "Hot reloading module: %s", nom_module);

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

    log_fmt(NiveauLogInfo, "Module '%s' reloaded successfully", nom_module);
    free_gestion_echec_compteur((char *)nom_module);
}

/*
 * Verifie periodiquement les fichiers Lua modifies et les recharge.
 * Appele a chaque frame, declenche le rechargement selon la frequence SECONDES_RELOAD_LUA.
 * Fonction de mise a jour pour le systeme de hot reload.
 */
void actualiser_rechargement(void) {
    Uint32 frame = gs->timing->compteur_frames;
    float fps = gs->timing->fps;
    Fichiers *a_modifier = NULL;

    if (frame % ((int)SDL_roundf(SECONDES_RELOAD_LUA * fps)) == 0 && frame != 0) {
        /* Premiere initialisation */
        if (!gs->frame->fichiers_lua) {
            gs->frame->fichiers_lua = renvoie_fichier_dossier("./src", "lua", NULL);
            return;
        }

        /* Rechargement des fichiers modifies */
        a_modifier = renvoie_fichier_modifie(gs->frame->fichiers_lua);
        for (int i = 0; i < a_modifier->taille; i++) {
            log_fmt(NiveauLogInfo, "File %s is modified, reloading it", a_modifier->noms[i]);
            recharger_lua(a_modifier->noms[i]);
        }
        liberer_fichiers(a_modifier);
    }
}