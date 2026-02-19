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
    uint8_t cle[16];   /* cle de chiffrement */
    uint8_t iv[16];    /* iv de chiffrement */
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
void initialiser_gestionnaire_son();

/* Charge tous les sons d'un dossier */
void charger_tout_les_sons(const char *dossier);

/* Fonctions de liberation */
void liberer_gestionnaire_son(void);
void liberer_sons(void);

/* mettre cle chiffrement */
void mettre_son_cle(int index, int valeur);

/* mettre iv chiffrement */
void mettre_son_iv(int index, int valeur);

#ifdef __cplusplus
}
#endif

#endif /* SON_H */