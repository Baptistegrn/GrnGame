#ifndef CHEMIN_H
#define CHEMIN_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../proprietes.h"

typedef struct Fichiers {
    int taille;
    int capacite;
    char (*noms)[TAILLE_LIEN_GT];
    long *temps;
} Fichiers;

char *normaliser_nom(const char *src);
char *normaliser_chemin(char *src);
Fichiers *renvoie_fichier_dossier(const char *chemin_dossier, const char *extension,
                                  Fichiers *fichiers);
void normaliser_chemin_copies(char *dst, const char *src);
void liberer_fichiers(Fichiers *fichiers);
const char *recupere_module(const char *chemin);
long renvoie_temp_fichier(const char *path);
Fichiers *renvoie_fichier_modifie(Fichiers *src);
#ifdef __cplusplus
}
#endif

#endif /* CHEMIN_H */