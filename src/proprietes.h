/*
 * Fichier de configuration des constantes du moteur GrnGame.
 * Contient les limites et valeurs par defaut utilisees dans tout le projet.
 */

#ifndef PROPRIETES_H
#define PROPRIETES_H

/* Taille maximale d'un chemin vers une ressource (image, son, etc.) */
#define TAILLE_LIEN 255

/* Nombre de canaux audio alloues pour SDL_mixer */
#define TAILLE_CANAL 32

/* Nombre maximum de manettes supportees simultanement */
#define NB_MANETTES_MAX 4

/* Taille maximale d'une ligne lors du chargement de carte */
#define TAILLE_LIGNE 16384

/* Delai en secondes entre les verifications de modifications des fichiers Lua */
#define SECONDES_RELOAD_LUA 1

/* Nombre maximum de particules dessinees en une seule fois */
#define MAX_PARTICULES 4096

/*
 * Cle de chiffrement XOR pour les scripts Lua.
 * Cette valeur doit etre modifiee pour chaque projet en production.
 * Voir encrypt.py dans moteur_lua pour le chiffrement des scripts.
 */
#ifndef GRNGAME_CRYPTED_KEY
#define GRNGAME_CRYPTED_KEY 0x42
#endif

#endif /* PROPRIETES_H */