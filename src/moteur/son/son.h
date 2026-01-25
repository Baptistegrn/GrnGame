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
    char id[TAILLE_LIEN_GT];
    Mix_Chunk *son;
} SonEntry;

typedef struct GestionnaireSon {
    SonEntry *entrees;
    int taille;
    int capacite;
} GestionnaireSon;

void jouer_son(const char *lien, int boucle, int canal, int volume);
void arreter_canal(int canal);
void arreter_son(const char *lien);
void pause_canal(int canal);
void pause_son(const char *lien);
void reprendre_canal(int canal);
void reprendre_son(const char *lien);

Mix_Chunk *recuperer_son_par_lien(const char *lien);
void init_gestionnaire_son();
void charger_tous_les_sons(const char *dossier);
void liberer_gestionnaire_son();

#ifdef __cplusplus
}
#endif

#endif /* SON_H */