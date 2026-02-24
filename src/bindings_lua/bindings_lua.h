/*
 * Enregistrement des bindings Lua.
 */

#ifndef BINDINGS_LUA_H
#define BINDINGS_LUA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Enregistre tous les bindings dans l'etat Lua passe en parametre. */
void enregistrer_bindings(void *L);

#ifdef __cplusplus
}
#endif

#endif /* BINDINGS_LUA_H */
