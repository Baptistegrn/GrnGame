/*
 * Gestion audio et des ressources sonores.
 */

#ifndef SON_H
#define SON_H

#include "../../proprietes.h"
#include <SDL_mixer.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SonEntry {
    char id[TAILLE_LIEN]; /* lien */
    Mix_Chunk *son;       /* son */
} SonEntry;

typedef struct GestionnaireSon {
    SonEntry *entrees; /* tableau des sons */
    int taille;        /* nombre de sons charges */
    int capacite;      /* capacite du tableau */
} GestionnaireSon;

/* Joue un son avec boucle, canal et volume specifies */
void jouer_son(const char *lien, int boucle, int canal, int volume);

/* Arrete la lecture sur un canal specifique */
void arreter_canal(int canal);

/* Arrete la lecture d'un son par son chemin */
void arreter_son(const char *lien);

/* Met en pause un canal specifique */
void pause_canal(int canal);

/* Met en pause un son par son chemin */
void pause_son(const char *lien);

/* Reprend la lecture sur un canal */
void reprendre_canal(int canal);

/* Reprend la lecture d'un son par son chemin */
void reprendre_son(const char *lien);

/* Recupere un son par son chemin */
Mix_Chunk *recuperer_son_par_lien(const char *lien);

/* Initialise le gestionnaire de sons */
void init_gestionnaire_son();

/* Charge tous les sons d'un dossier */
void charger_tous_les_sons(const char *dossier);

/* Fonctions de liberation */
void liberer_gestionnaire_son(void);
void liberer_sons(void);

#ifdef __cplusplus
}
#endif

#endif /* SON_H */