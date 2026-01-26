#include "../main.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

lua_State *G_LuaState = NULL;

// la cle peut etre change on genere dans le moteur par lutilisateur (a lui de le faire)
unsigned char key = 0x42;

void initializeLua(const char *fichier_chemin_lua) {
#ifdef DEBUG_MODE
    initialiser_logging(DestinationLogFichier, "../src/logs/game.logs", NiveauLogInfo);
#endif

    log_fmt(NiveauLogInfo, "Initializing Lua");
    G_LuaState = luaL_newstate();
    luaL_openlibs(G_LuaState);
    log_fmt(NiveauLogInfo, "Lua state created");

    const char *filename = fichier_chemin_lua;
    log_fmt(NiveauLogInfo, "Loading Lua file: %s", filename);

    FILE *f = fopen(filename, "rb");
    if (!f) {
        log_fmt(NiveauLogErreur, "Failed to open Lua file: %s", filename);
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(size);
    if (!buffer) {
        log_fmt(NiveauLogErreur, "Memory allocation failed for Lua buffer");
        fclose(f);
        return;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    unsigned char signature[] = {0x1B, 0x45, 0x4E, 0x43};
    int est_chiffre = (size > 4 && memcmp(buffer, signature, 4) == 0);

    if (est_chiffre) {
        log_fmt(NiveauLogInfo, "Encrypted Lua script detected, decrypting");
        for (long i = 4; i < size; i++) {
            buffer[i] ^= key;
        }
    }

    const char *buffer_a_charger = est_chiffre ? (const char *)(buffer + 4) : (const char *)buffer;
    size_t taille_a_charger = est_chiffre ? size - 4 : size;
    int status = luaL_loadbuffer(G_LuaState, buffer_a_charger, taille_a_charger, filename);

    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Lua Syntax Error: %s", lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1); // On retire l'erreur de la pile
        free(buffer);
        return;
    }

    log_fmt(NiveauLogInfo, "Executing Lua script...");
    status = lua_pcall(G_LuaState, 0, LUA_MULTRET, 0);

    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Lua Runtime Error: %s", lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1); // On retire l'erreur
    } else {
        log_fmt(NiveauLogInfo, "Lua script executed successfully");
    }

    free(buffer);
    log_fmt(NiveauLogInfo, "Lua initialization finished");
}