/*
 * Module de bindings Lua .
 * Gere l'initialisation, le rechargement a chaud (debug) et la liberation des scripts Lua.
 */

#ifndef BINDINGS_LUA_H
#define BINDINGS_LUA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialise la VM Lua et charge un script.
 * Detecte automatiquement si le script est chiffre.
 * En mode DEBUG_MODE, initialise le logging vers un fichier.
 */
void initialiser_lua(const char *fichier_chemin_lua);

/*
 * Enregistre tous les bindings GrnGame dans l'etat Lua.
 * Expose les API C dans le namespace "Grn" pour les scripts Lua.
 */
void enregistrer_bindings(void *L);

/*
 * Recharge un module Lua a chaud (hot reload).
 * Ne peut pas recharger main.lua (point d'entree).
 * Invalide le cache dans package.loaded avant recharge.
 */
void recharger_lua(const char *chemin);

/*
 * Verifie periodiquement les fichiers Lua modifies.
 * Appele a chaque frame, declenche le rechargement selon SECONDES_RELOAD_LUA.
 * Fonction de mise a jour pour le hot reload en mode debug.
 */
void actualiser_rechargement(void);

/*
 * Libere les ressources liees au hot reload des fichiers Lua.
 * Appellee lors de la destruction du moteur.
 */
void liberer_fichiers_lua(void);

/*
 * Ferme l'etat Lua global.
 * Appele a la fin du programme pour liberer les ressources Lua.
 */
void liberer_lua(void);

/*
 * Reinitialise la fonction de mise a jour Lua.
 * Doit etre appele avant lua_close() pour eviter des acces invalides.
 */
void fermer_callback(void);

#ifdef __cplusplus
}
#endif

#endif /* BINDINGS_LUA_H */