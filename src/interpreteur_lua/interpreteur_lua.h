/*
 * Initialisation et cycle de vie de l'interpreteur Lua.
 */

#ifndef INTERPRETEUR_LUA_H
#define INTERPRETEUR_LUA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Initialise la VM Lua et charge le script indique. */
void initialiser_lua(const char *fichier_chemin_lua);

/* Ferme et libere l'etat Lua global. */
void liberer_lua(void);

/* Reinitialise le callback de mise a jour Lua avant fermeture pour eviter des acces invalides. */
void fermer_callback(void);

#ifdef __cplusplus
}
#endif

#endif /* INTERPRETEUR_LUA_H */
