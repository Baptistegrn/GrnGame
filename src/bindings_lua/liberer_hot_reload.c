/*
 * Liberation des ressources liees au hot reload Lua.
 * Nettoie la liste des fichiers Lua charges et surveilles.
 */

#include "../main.h"
#include <lua.h>

extern lua_State *G_LuaState;

/*
 * Libere les fichiers Lua charges pour le hot reload.
 * Appele lors de la destruction du moteur.
 */
void liberer_fichiers_lua(void) {
    if (!gs)
        return;

    if (gs->frame->fichiers_lua != NULL) {
        liberer_fichiers(gs->frame->fichiers_lua);
        gs->frame->fichiers_lua = NULL;
    }
}

/*
 * Ferme l'etat Lua global.
 * Appele a la fin du programme pour liberer les ressources Lua.
 */
void liberer_lua(void) {
    if (G_LuaState) {
        log_fmt(NiveauLogInfo, "Closing Lua state...");
        lua_close(G_LuaState);
        G_LuaState = NULL;
        log_fmt(NiveauLogInfo, "Lua state closed");
    }
}
