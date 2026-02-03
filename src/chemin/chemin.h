/*
 * Utilitaires pour la gestion des chemins de fichiers.
 * Normalisation, parcours de dossiers et detection de modifications.
 */

#ifndef CHEMIN_H
#define CHEMIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../proprietes.h"

/* Structure contenant une liste de fichiers avec leurs dates de modification */
typedef struct Fichiers {
    int taille;                /* Nombre de fichiers */
    int capacite;              /* Capacite du tableau */
    char (*noms)[TAILLE_LIEN]; /* Tableau des chemins */
    long *temps;               /* Tableau des dates de modification */
} Fichiers;

/* Convertit une chaine en minuscules (alloue une nouvelle chaine) */
char *normaliser_nom(const char *src);

/* Remplace les \\ par des / dans une chaine (modifie sur place) */
char *normaliser_chemin(char *src);

/* Copie src dans dst en normalisant les separateurs */
void normaliser_chemin_copies(char *dst, const char *src);

/* Parcourt un dossier et retourne les fichiers avec l'extension donnee */
Fichiers *renvoie_fichier_dossier(const char *chemin_dossier, const char *extension,
                                  Fichiers *fichiers);

/* Libere une structure Fichiers */
void liberer_fichiers(Fichiers *fichiers);

/* Extrait le nom du module Lua a partir d'un chemin */
const char *recupere_module(const char *chemin);

/* Retourne la date de derniere modification d'un fichier */
long renvoie_temp_fichier(const char *path);

/* Detecte les fichiers modifies depuis le dernier scan */
Fichiers *renvoie_fichier_modifie(Fichiers *src);

#ifdef __cplusplus
}
#endif

#endif /* CHEMIN_H */