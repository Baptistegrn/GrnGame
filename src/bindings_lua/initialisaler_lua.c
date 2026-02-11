/*
 * Initialisation et execution de la VM Lua.
 * Gere le chargement des scripts Lua (source ou bytecode) et leur execution.
 */

#include "../main.h"
#include "bindings_lua.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

/* Etat global Lua partage dans tout le moteur */
lua_State *G_LuaState = NULL;

/*
 * Initialise la VM Lua et charge un script.
 * Supporte le code source Lua et le bytecode (luac).
 * En mode DEBUG_MODE, initialise le logging vers un fichier.
 */
void initialiser_lua(const char *fichier_chemin_lua) {
#ifdef DEBUG_MODE
    initialiser_logging(DestinationLogFichier, "./src/logs/game.logs", NiveauLogInfo);
#endif

    log_fmt(NiveauLogInfo, "Initializing Lua");
    G_LuaState = luaL_newstate();
    luaL_openlibs(G_LuaState);
    log_fmt(NiveauLogInfo, "Lua state created");

    /* Enregistrement des bindings GrnGame */
    enregistrer_bindings(G_LuaState);
    log_fmt(NiveauLogInfo, "GrnGame bindings registered");

    const char *filename = fichier_chemin_lua;
    log_fmt(NiveauLogInfo, "Loading Lua file: %s", filename);

    /* Ouverture du fichier en mode binaire */
    FILE *f = fopen(filename, "rb");
    if (!f) {
        log_fmt(NiveauLogErreur, "Failed to open Lua file: %s", filename);
        return;
    }

    /* Lecture de la taille du fichier */
    fseek(f, 0, SEEK_END);
    long taille_fichier = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* Allocation du buffer pour le contenu */
    unsigned char *buffer = malloc_gestion_echec_compteur(taille_fichier);
    if (!buffer) {
        log_fmt(NiveauLogErreur, "Memory allocation failed for Lua buffer");
        fclose(f);
        return;
    }

    fread(buffer, 1, taille_fichier, f);
    fclose(f);

    /* Chargement du script (source ou bytecode, gere automatiquement par Lua) */
    int status = luaL_loadbuffer(G_LuaState, (const char *)buffer, taille_fichier, filename);

    if (status != LUA_OK) {
        /* syntaxe en dehors de la boucle update */
        log_fmt(NiveauLogErreur, "Lua Syntax Error: %s", lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
        free(buffer);
        return;
    }

    /* Execution du script */
    log_fmt(NiveauLogInfo, "Executing Lua script...");
    status = lua_pcall(G_LuaState, 0, LUA_MULTRET, 0);

    if (status != LUA_OK) {
        /* erreur en dehors de la boucle update */
        log_fmt(NiveauLogErreur, "Lua Runtime Error: %s", lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
    } else {
        log_fmt(NiveauLogInfo, "Lua script executed successfully");
    }

    free_gestion_echec_compteur(buffer);

    log_fmt(NiveauLogInfo, "Lua initialization finished");
}