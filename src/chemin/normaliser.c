/*
 * Fonctions de normalisation des noms et chemins.
 */

#include "../allouer/allouer.h"
#include "../proprietes.h"
#include <ctype.h>
#include <string.h>


/*
 * Convertit un nom en minuscules.
 * Retourne une nouvelle chaine allouee a liberer par l'appelant.
 */
char *normaliser_nom(const char *src) {
    if (!src)
        return NULL;

    int taille = strlen(src);
    char *dst = malloc_gestion_echec_compteur(sizeof(char) * (taille + 1));

    for (int i = 0; i < taille; i++) {
        dst[i] = tolower(src[i]);
    }
    dst[taille] = '\0';

    return dst;
}

/*
 * Normalise un chemin en remplacant les \\ par des /.
 * Modifie la chaine sur place et retourne le meme pointeur.
 */
char *normaliser_chemin(char *str) {
    if (!str)
        return NULL;

    char *p = str;
    while (*p) {
        if (*p == '\\') {
            *p = '/';
        }
        p++;
    }
    return str;
}

/*
 * Copie src dans dst en normalisant les separateurs.
 * Supprime les doubles slashs et limite a TAILLE_LIEN caracteres.
 */
void normaliser_chemin_copies(char *dst, const char *src) {
    if (!src || !dst)
        return;

    int i = 0;
    int j = 0;
    while (src[i] != '\0' && j < TAILLE_LIEN - 1) {
        char c = src[i];
        if (c == '\\') {
            c = '/';
        }
        /* Evite les doubles slashs */
        if (c == '/' && j > 0 && dst[j - 1] == '/') {
            i++;
            continue;
        }
        dst[j] = c;
        i++;
        j++;
    }

    dst[j] = '\0';
}
