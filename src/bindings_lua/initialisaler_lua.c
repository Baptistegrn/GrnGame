/*
 * Initialisation et execution de la VM Lua.
 * Gere le chargement des scripts Lua (chiffres ou non) et leur execution.
 * Detecte automatiquement les fichiers chiffres via une signature.
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
 * Detecte automatiquement les scripts chiffres (signature 0x1B 0x45 0x4E 0x43).
 * En mode DEBUG_MODE, initialise le logging vers un fichier.
 */
void initialiser_lua(const char *fichier_chemin_lua) {
#ifdef DEBUG_MODE
    initialiser_logging(DestinationLogFichier, "../src/logs/game.logs", NiveauLogInfo);
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
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* Allocation du buffer pour le contenu */
    unsigned char *buffer = (unsigned char *)malloc(size);
    if (!buffer) {
        log_fmt(NiveauLogErreur, "Memory allocation failed for Lua buffer");
        fclose(f);
        return;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    /* Detection de la signature de chiffrement */
    unsigned char signature[] = {0x1B, 0x45, 0x4E, 0x43};
    bool est_chiffre = (size > 4 && memcmp(buffer, signature, 4) == 0);

    /* Dechiffrement XOR si le fichier est chiffre */
    if (est_chiffre) {
        log_fmt(NiveauLogInfo, "Encrypted Lua script detected, decrypting");
        for (long i = 4; i < size; i++) {
            buffer[i] ^= GRNGAME_CRYPTED_KEY;
        }
    }

    /* Chargement du script dans la VM Lua */
    const char *buffer_a_charger = est_chiffre ? (const char *)(buffer + 4) : (const char *)buffer;
    size_t taille_a_charger = est_chiffre ? size - 4 : size;
    int status = luaL_loadbuffer(G_LuaState, buffer_a_charger, taille_a_charger, filename);

    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Lua Syntax Error: %s", lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
        free(buffer);
        return;
    }

    /* Execution du script */
    log_fmt(NiveauLogInfo, "Executing Lua script...");
    status = lua_pcall(G_LuaState, 0, LUA_MULTRET, 0);

    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Lua Runtime Error: %s", lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
    } else {
        log_fmt(NiveauLogInfo, "Lua script executed successfully");
    }

    free(buffer);

    /* Fermeture de l'etat Lua APRES que lua_pcall a retourne
     * pour eviter un crash lors du retour de la pile d'appels */
    liberer_lua();

    log_fmt(NiveauLogInfo, "Lua initialization finished");
}