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
#define SECONDES_RELOAD_LUA 0.1f

/* Nombre maximum de particules dessinees en une seule fois */
#define MAX_PARTICULES 4096

/* parametres par default des fonctions de bindings lua ( peut etre modifier ).*/

/* image */
#define DEFAULT_ROTATION 0
#define DEFAULT_ROTATIONP 0
#define DEFAULT_ALPHA 255
#define DEFAULT_R 124
#define DEFAULT_G 124
#define DEFAULT_B 124
#define DEFAULT_SPACING 1
#define DEFAULT_FLIP false

/* module jeu */
#define DEFAULT_VMAX_CHUTE 300.0f
#define DEFAULT_CORRECTION_MUR 150.0f
#define DEFAULT_GRAVITE 300.0f
#define DEFAULT_FORCE_SAUT -150.0f
#define DEFAULT_CAMERA_SMOOTH 2.5f

/* son */
#define DEFAULT_LOOP 0
#define DEFAULT_CHANNEL 1
#define DEFAULT_VOLUME 124

#endif /* PROPRIETES_H */