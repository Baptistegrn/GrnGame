#include "../main.h"
#include "chemin.h"
#include <stdlib.h>

/* renvoie la derniere modification dun fichier */
long renvoie_temp_fichier(const char *path) {
#if defined(_WIN32)
#include "windows.h"
    WIN32_FILE_ATTRIBUTE_DATA fichier_info;
    if (GetFileAttributesExA(path, GetFileExInfoStandard, &fichier_info)) {
        ULARGE_INTEGER uli;
        uli.LowPart = fichier_info.ftLastWriteTime.dwLowDateTime;
        uli.HighPart = fichier_info.ftLastWriteTime.dwHighDateTime;
        return (long)(uli.QuadPart / 10000000ULL);
    }
#else
#include <sys/types.h>
    struct stat attr;
    if (stat(path, &attr) == 0) {
        return (long)attr.st_mtime;
    }
#endif
    return -1;
}

/* renvoie chaques fichiers modifie par lutilisateur selon une liste de fichier src,corrige les
 * fichiers sources .
 */
Fichiers *renvoie_fichier_modifie(Fichiers *src) {
    Fichiers *resultat = xmalloc(sizeof *resultat);

    resultat->capacite = 3;
    resultat->taille = 0;
    resultat->noms = xmalloc(resultat->capacite * sizeof *resultat->noms);
    resultat->temps = xmalloc(resultat->capacite * sizeof *resultat->temps);

    for (int i = 0; i < src->taille; i++) {
        long nv = renvoie_temp_fichier(src->noms[i]);

        if (nv > src->temps[i]) {
            log_fmt(NiveauLogDebug, "New modifition detected : old time: %li, new time : %li",
                    src->temps[i], nv);
            if (resultat->taille >= resultat->capacite) {
                resultat->capacite *= 2;
                resultat->noms =
                    xrealloc(resultat->noms, resultat->capacite * sizeof *resultat->noms);
                resultat->temps =
                    xrealloc(resultat->temps, resultat->capacite * sizeof *resultat->temps);
            }

            strcpy(resultat->noms[resultat->taille], src->noms[i]);
            resultat->temps[resultat->taille] = nv;
            resultat->taille++;
            src->temps[i] = nv;
        }
    }

    return resultat;
}
