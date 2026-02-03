/*
 * Liberation des ressources liees aux fichiers Lua.
 */

#include "../main.h"

/* libere les fichiers Lua charge a chaques secondes pour le hot reload */
void liberer_fichiers_lua(void) {
    if (!gs)
        return;
    if (gs->frame->fichiers_lua != NULL) {
        liberer_fichiers(gs->frame->fichiers_lua);
        gs->frame->fichiers_lua = NULL;
    }
}
