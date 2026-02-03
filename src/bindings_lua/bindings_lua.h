/*
 * Module de bindings Lua.
 * Gere l'initialisation, le rechargement a chaud en debug et la liberation des scripts Lua.
 */

#ifndef BINDINGS_LUA_H
#define BINDINGS_LUA_H

/* Initialise et execute un script Lua */
void initialiser_lua(const char *fichier_chemin_lua);

/* Recharge un module Lua a chaud (hot reload) */
void recharger_lua(const char *chemin);

/* Verifie et recharge les fichiers Lua modifies */
void actualiser_rechargement(void);

/* Libere les ressources liees aux fichiers Lua */
void liberer_fichiers_lua(void);

#endif /* BINDINGS_LUA_H */