/*
 * Fichier de configuration des constantes du moteur GrnGame.
 * Contient les limites et valeurs par defaut utilisees dans tout le projet.
 */

#ifndef PROPRIETES_H
#define PROPRIETES_H

#ifdef __cplusplus
extern "C" {
#endif

/* intervalle sur lequel on affiche le compteur dallocations en debug */
#define AFFICHAGE_COMPTEUR 20

/* Taille maxmale d'un chemin json */
#define TAILLE_LIEN_JSON 1024

/* Taille maximale d'un chemin vers une ressource (image, son, etc.) */
#define TAILLE_LIEN 512

/* Nombre de canaux audio alloues pour SDL_mixer */
#define TAILLE_CANAL 32

/* Nombre maximum de manettes supportees simultanement */
#define NB_MANETTES_MAX 4

/* Taille maximale d'une ligne lors du chargement de carte */
#define TAILLE_LIGNE 8192

/* Delai en secondes entre les verifications de modifications des fichiers Lua */
#define SECONDES_RELOAD_LUA 0.2f

/* Nombre maximum de particules dessinees en une seule fois */
#define MAX_PARTICULES 4096

/* parametres par default des fonctions de bindings lua ( peut etre modifier ).*/

/* fenetre */
#define DEFAULT_HEIGHT 320
#define DEFAULT_WIDTH 180
#define DEFAULT_FPS 60.0f
#define DEFAULT_BLACK_BARS true
#define DEFAULT_WINDOW_TITLE "window"
#define DEFAULT_UPDATE_FUNC "update"

/* image */
#define DEFAULT_ROTATION 0
#define DEFAULT_ROTATIONP 0
#define DEFAULT_ALPHA 255
#define DEFAULT_R 255
#define DEFAULT_G 0
#define DEFAULT_B 0
#define DEFAULT_SPACING 1
#define DEFAULT_FLIP false

/* module jeu */
#define DEFAULT_VMAX_CHUTE 300.0f
#define DEFAULT_VMAX_X 3.0f
#define DEFAULT_ACCELERATION 8.0f
#define DEFAULT_VITESSE_INITIALE 0.4f
#define DEFAULT_CORRECTION_MUR 150.0f
#define DEFAULT_GRAVITE 300.0f
#define DEFAULT_FORCE_SAUT -150.0f
#define DEFAULT_CAMERA_SMOOTH 2.5f
#define DEFAULT_NB_SAUTS_MAX 1
#define DEFAULT_SAUT_SUR_MURS false

/* hitbox */
#define TOLERANCE_MUR 0.15f
#define TOLERANCE_SOL 0.10f
#define DUREE_BLOCAGE_MURAL 0.20f

/* son */
#define DEFAULT_LOOP 0
#define DEFAULT_CHANNEL 1
#define DEFAULT_VOLUME 64

#ifdef __cplusplus
}
#endif

#endif /* PROPRIETES_H */