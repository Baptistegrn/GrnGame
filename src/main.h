/*
 * En-tete principal du moteur GrnGame.
 * Centralise tous les includes necessaires pour eviter les dependances manuelles.
 * Chaque fichier source inclut uniquement main.h.
 */

#ifndef MAIN_H
#define MAIN_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

/* Bibliotheques SDL */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/* Bibliotheques C standard */
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Bibliotheques specifiques a la plateforme */
#ifdef _WIN32
#include <windows.h>
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#else
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Modules internes du moteur */
#include "allouer/allouer.h"
#include "bindings_lua/bindings_lua.h"
#include "chemin/chemin.h"
#include "liberer/liberer.h"
#include "module_jeu/camera/camera.h"
#include "module_jeu/carte/carte.h"
#include "module_jeu/hitbox/hitbox.h"
#include "moteur/boucle/boucle.h"
#include "moteur/constante/constante.h"
#include "moteur/entrees/entrees.h"
#include "moteur/fenetre/fenetre.h"
#include "moteur/image/affichage/affichage.h"
#include "moteur/image/ajout/ajout.h"
#include "moteur/image/bande_noir/bande_noir.h"
#include "moteur/image/caractere/caractere.h"
#include "moteur/image/chargement/chargement_image.h"
#include "moteur/image/dessin_forme/dessin.h"
#include "moteur/image/liste/liste_chaine.h"
#include "moteur/image/rotation/rotation.h"
#include "moteur/initialisation/initialisation.h"
#include "moteur/json/json.h"
#include "moteur/logging/logging.h"
#include "moteur/son/son.h"


/*
 * Structure principale du moteur.
 * Contient tous les gestionnaires necessaires au fonctionnement du jeu.
 */
typedef struct Gestionnaire {
    GestionnaireTiming *timing;       /* Gestion du temps et de la boucle */
    GestionnaireFenetre *fenetre;     /* Gestion de la fenetre SDL */
    GestionnaireEntrees *entrees;     /* Gestion des entrees clavier, souris, manettes */
    GestionnaireTextures *textures;   /* Cache des textures chargees */
    GestionnaireSon *sons;            /* Gestion des sons et musiques */
    GestionnaireFrameActuelle *frame; /* Donnees de la frame courante */
    GestionnaireFichiers *fichiers;
} Gestionnaire;

/* Instance globale du gestionnaire */
extern Gestionnaire *gs;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
