/*
 * Gestion du rechargement a chaud des scripts Lua.
 */

#ifndef HOT_RELOAD_H
#define HOT_RELOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Recharge un module Lua (hot reload). Ne recharge pas main.lua */
void recharger_lua(const char *chemin);

/* Verifie a chaque frame si des fichiers Lua ont ete modifies et les recharge */
void actualiser_rechargement(void);

/* Libere les ressources utilises pour le suivi des fichiers Lua */
void liberer_fichiers_lua(void);

#ifdef __cplusplus
}
#endif

#endif /* HOT_RELOAD_H */
