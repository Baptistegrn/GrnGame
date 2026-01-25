// #include <stdio.h>
// #include <stdlib.h>

// #include <lauxlib.h>
// #include <lua.h>
// #include <lualib.h>

// // Déclaration externe de la fonction d'enregistrement des bindings
// extern void enregistrer_bindings(lua_State *L);

// // État global de Lua pour utilisation ailleurs
// lua_State *G_LuaState = NULL;

// /*
//  * Fonction interne pour initialiser le système Lua.
//  * Crée la machine virtuelle, ouvre les bibliothèques standard,
//  * enregistre les bindings C et charge le script main.lua.
//  */
// void initialisation_lua() {
//     // Création de la machine virtuelle Lua
//     G_LuaState = luaL_newstate();
//     if (G_LuaState == NULL) {
//         printf("Echec creation lua_State\n");
//         return;
//     }

//     // Ouverture des bibliothèques standard Lua
//     luaL_openlibs(G_LuaState);

//     // Enregistrement des fonctions C exposées à Lua
//     enregistrer_bindings(G_LuaState);

//     // Chargement et exécution du script principal main.lua
//     if (luaL_dofile(G_LuaState, "main.lua") != LUA_OK) {
//         const char *erreur = lua_tostring(G_LuaState, -1);
//         printf("%s\n", erreur);
//         lua_pop(G_LuaState, 1);
//     }
// }