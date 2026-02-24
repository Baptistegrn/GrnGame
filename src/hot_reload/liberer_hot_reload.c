/*
 * Liberation des ressources liees au hot reload Lua.
 * Nettoie la liste des fichiers Lua charges et surveilles.
 */

#include "../chemin/chemin.h"
#include "../interpreteur_lua/interpreteur_lua.h"
#include "../main.h"
#include "../moteur/boucle/boucle.h"
#include "../moteur/logging/logging.h"
#include "../prediction_branche.h"
#include "hot_reload.h"

#include <lua.h>

extern lua_State *G_LuaState;

/*
 * Libere les fichiers Lua charges pour le hot reload.
 * Appele lors de la destruction du moteur.
 */
void liberer_fichiers_lua(void) {
    if (UNLIKELY(!gs))
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
        log_fmt(NiveauLogInfo, "closing Lua state...");
        /* Reset des objets sol avant de fermer l'etat */
        fermer_callback();
        lua_close(G_LuaState);
        G_LuaState = NULL;
        log_fmt(NiveauLogInfo, "lua state closed");
    }
}
