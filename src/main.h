/*
 * le main.h permet ici de tout inclure dans les fichiers sans chaques dependences a la main.
 */

#ifndef MAIN_H
#define MAIN_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "allouer/allouer.h"
#include "chemin/chemin.h"
#include "liberer/liberer.h"
#include "module_jeu/camera/camera.h"
#include "module_jeu/carte/carte.h"
#include "module_jeu/hitbox/hitbox.h"
#include "moteur/boucle/boucle.h"
#include "moteur/constante/constante.h"
#include "moteur/fenetre/fenetre.h"
#include "moteur/image/affichage/affichage.h"
#include "moteur/image/ajout/ajout.h"
#include "moteur/image/bande_noir/bande_noir.h"
#include "moteur/image/caractere/caractere.h"
#include "moteur/image/chargement/chargement_image.h"
#include "moteur/image/forme/dessin.h"
#include "moteur/image/liste/liste_chaine.h"
#include "moteur/image/rotate/rotate.h"
#include "moteur/initialisation/initialisation.h"
#include "moteur/input/input.h"
#include "moteur/logging/logging.h"
#include "moteur/son/son.h"

typedef struct Gestionnaire {
    GestionnaireTiming *timing;
    GestionnaireFenetre *fenetre;
    GestionnaireEntrees *entrees;
    GestionnaireTextures *textures;
    GestionnaireSon *sons;
    // todo mettre dans GestionnaireFrame
    fondActualiser *fond;
    TableauImage *image;
} Gestionnaire;

extern Gestionnaire *gs;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#endif

#endif
