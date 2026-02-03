/*
 * Detection des fichiers modifies.
 * Utilise pour le hot reload des scripts Lua en mode debug.
 */

#include "../main.h"
#include "chemin.h"

/* Retourne la date de derniere modification d'un fichier */
long renvoie_temp_fichier(const char *path) {
#if defined(_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA fichier_info;
    if (GetFileAttributesExA(path, GetFileExInfoStandard, &fichier_info)) {
        ULARGE_INTEGER uli;
        uli.LowPart = fichier_info.ftLastWriteTime.dwLowDateTime;
        uli.HighPart = fichier_info.ftLastWriteTime.dwHighDateTime;
        return (long)(uli.QuadPart / 10000000ULL);
    }
#else
    struct stat attr;
    if (stat(path, &attr) == 0) {
        return (long)attr.st_mtime;
    }
#endif
    return -1;
}

/*
 * Compare les dates de modification et retourne les fichiers modifies.
 * Met a jour les dates dans la structure source.
 */
Fichiers *renvoie_fichier_modifie(Fichiers *src) {
    Fichiers *resultat = malloc_gestion_echec_compteur(sizeof *resultat);

    resultat->capacite = 3;
    resultat->taille = 0;
    resultat->noms = malloc_gestion_echec_compteur(resultat->capacite * sizeof *resultat->noms);
    resultat->temps = malloc_gestion_echec_compteur(resultat->capacite * sizeof *resultat->temps);

    for (int i = 0; i < src->taille; i++) {
        long nv = renvoie_temp_fichier(src->noms[i]);

        if (nv > src->temps[i]) {
            log_fmt(NiveauLogDebug, "New modifition detected : old time: %li, new time : %li",
                    src->temps[i], nv);

            /* Agrandissement si necessaire */
            if (resultat->taille >= resultat->capacite) {
                resultat->capacite *= 2;
                resultat->noms = realloc_gestion_echec_compteur(
                    resultat->noms, resultat->capacite * sizeof *resultat->noms);
                resultat->temps = realloc_gestion_echec_compteur(
                    resultat->temps, resultat->capacite * sizeof *resultat->temps);
            }

            strcpy(resultat->noms[resultat->taille], src->noms[i]);
            resultat->temps[resultat->taille] = nv;
            resultat->taille++;

            /* Mise a jour de la date source */
            src->temps[i] = nv;
        }
    }

    return resultat;
}
