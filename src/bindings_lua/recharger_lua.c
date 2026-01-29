
#include "../main.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

extern lua_State *G_LuaState;
/*recharge un module lua en fonction du chemin donnee attention il ne peut pas recharger le point
 * dentree du programme et recharge que des packages
 */
void recharger_lua(const char *chemin) {
    if (!G_LuaState)
        return;
    char dst[TAILLE_LIEN_GT];
    // normalisation du chemin
    normaliser_chemin_copies(dst, chemin);
    // recuperation du module
    const char *nom_module = recupere_module(dst);
    if (!nom_module) {
        log_fmt(NiveauLogErreur, "Impossible to retrieve module name from path: %s", chemin);
        return;
    }
    // on skip le rechargement du main.lua
    if (strcmp(nom_module, "main") == 0) {
        log_message(NiveauLogInfo, "main.lua is modifing hot reload skiped...");
        xfree((char *)nom_module);
        return;
    }
    log_fmt(NiveauLogDebug, "recharging of module : %s", nom_module);
    // Invalider le module dans package.loaded
    lua_getglobal(G_LuaState, "package");     // stack: package
    lua_getfield(G_LuaState, -1, "loaded");   // stack: package, loaded
    lua_pushnil(G_LuaState);                  // stack: package, loaded, nil
    lua_setfield(G_LuaState, -2, nom_module); // package.loaded[nom_module] = nil
    lua_pop(G_LuaState, 2);                   // stack: (vide)

    int status = luaL_loadfile(G_LuaState, chemin);

    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Syntax error in %s: %s", chemin, lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
        xfree((char *)nom_module);
        return;
    }

    status = lua_pcall(G_LuaState, 0, 1, 0); // 1 valeur de retour (le module)

    if (status != LUA_OK) {
        log_fmt(NiveauLogErreur, "Runtime error in %s: %s", chemin, lua_tostring(G_LuaState, -1));
        lua_pop(G_LuaState, 1);
        xfree((char *)nom_module);
        return;
    }

    // Mettre à jour package.loaded avec le nouveau module
    lua_getglobal(G_LuaState, "package");     // stack: module, package
    lua_getfield(G_LuaState, -1, "loaded");   // stack: module, package, loaded
    lua_pushvalue(G_LuaState, -3);            // stack: module, package, loaded, module
    lua_setfield(G_LuaState, -2, nom_module); // package.loaded[nom_module] = module
    lua_pop(G_LuaState, 3);                   // stack: (vide)

    log_fmt(NiveauLogInfo, "Module '%s' reloaded", nom_module);
    xfree((char *)nom_module);
}
/* recupere chaqun des fichiers a recharger , gere le temp et appelle le rechargement de fichiers */
void actualiser_rechargement() {
    Uint32 frame = gs->timing->compteur_frames;
    float fps = gs->timing->fps;
    Fichiers *a_modifier = NULL;
    if (frame % (SECONDES_RELOAD_LUA * (int)fps) == 0 && frame != 0) {
        a_modifier = renvoie_fichier_modifie(gs->fichiers_lua);
        for (int i = 0; i < a_modifier->taille; i++) {
            log_fmt(NiveauLogInfo, "File  %s is modified ,reload it", a_modifier->noms[i]);
            recharger_lua(a_modifier->noms[i]);
        }
        liberer_fichiers(a_modifier);
    }
}