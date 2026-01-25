#include "../main.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

lua_State *G_LuaState = NULL;

// cle : peut etre change
unsigned char key = 0x42;

void initialisation_lua() {
    G_LuaState = luaL_newstate();
    luaL_openlibs(G_LuaState);

    const char *filename = "main.lua";
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("error : Impossible to open %s\n", filename);
        fflush(stdout);
        return;
    }

    // Mesurer la taille
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buffer = malloc(size);
    fread(buffer, 1, size, f);
    fclose(f);
    // signatures de cryptage
    unsigned char signature[] = {0x1B, 0x45, 0x4E, 0x43};

    int est_chiffre = (size > 4 && memcmp(buffer, signature, 4) == 0);

    const char *buffer_a_charger = (const char *)buffer;
    size_t taille_a_charger = size;

    if (est_chiffre) {
        for (long i = 4; i < size; i++) {
            buffer[i] ^= key;
        }
        buffer_a_charger = (const char *)(buffer + 4);
        taille_a_charger = size - 4;
    } else {
    }

    // chargement lua
    int status = luaL_loadbuffer(G_LuaState, buffer_a_charger, taille_a_charger, filename);

    if (status == LUA_OK) {
        lua_pcall(G_LuaState, 0, LUA_MULTRET, 0);
    } else {
        // erreur interpretation lua
        printf("error Lua : %s\n", lua_tostring(G_LuaState, -1));
        fflush(stdout);
    }

    free(buffer);
}