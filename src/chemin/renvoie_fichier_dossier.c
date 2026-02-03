/*
 * Parcours recursif de dossiers.
 * Implementation multi-plateforme (Windows et POSIX).
 */

#include "../main.h"

#ifdef _WIN32

/* Verifie si un fichier a l'extension donnee */
int est_extension(const char *nom, const char *ext) {
    const char *dot = strrchr(nom, '.');
    if (!dot || dot == nom)
        return 0;
    return strcmp(dot + 1, ext) == 0;
}

/*
 * Parcourt un dossier et ses sous-dossiers (Windows).
 * Retourne les fichiers correspondant a l'extension donnee.
 */
Fichiers *renvoie_fichier_dossier(const char *chemin_dossier, const char *extension,
                                  Fichiers *fichiers) {
    /* Premier appel : allocation de la structure */
    if (fichiers == NULL) {
        fichiers = malloc_gestion_echec_compteur(sizeof(Fichiers));
        fichiers->taille = 0;
        fichiers->capacite = 10;
        fichiers->noms =
            malloc_gestion_echec_compteur(fichiers->capacite * sizeof(*fichiers->noms));
        fichiers->temps =
            malloc_gestion_echec_compteur(fichiers->capacite * sizeof(*fichiers->temps));
    }

    char chemin_recherche[TAILLE_LIEN];
    snprintf(chemin_recherche, TAILLE_LIEN, "%s/*", chemin_dossier);

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(chemin_recherche, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        log_fmt(NiveauLogErreur, "Could not open directory (Win32): %s", chemin_dossier);
        return fichiers;
    }

    do {
        /* Ignore . et .. */
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
            continue;
        }

        char chemin_complet[TAILLE_LIEN];
        int len =
            snprintf(chemin_complet, TAILLE_LIEN, "%s/%s", chemin_dossier, findData.cFileName);

        if (len >= TAILLE_LIEN) {
            log_fmt(NiveauLogAvertissement, "Path too long: %s/%s", chemin_dossier,
                    findData.cFileName);
            continue;
        }

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            /* Recursion dans les sous-dossiers */
            renvoie_fichier_dossier(chemin_complet, extension, fichiers);
        } else {
            if (est_extension(findData.cFileName, extension)) {
                /* Agrandissement si necessaire */
                if (fichiers->taille >= fichiers->capacite) {
                    fichiers->capacite *= 2;
                    fichiers->noms = realloc_gestion_echec_compteur(
                        fichiers->noms, fichiers->capacite * sizeof(*fichiers->noms));
                    fichiers->temps = realloc_gestion_echec_compteur(
                        fichiers->temps, fichiers->capacite * sizeof(*fichiers->temps));
                }
                snprintf(fichiers->noms[fichiers->taille], TAILLE_LIEN, "%s", chemin_complet);
                fichiers->temps[fichiers->taille] = renvoie_temp_fichier(chemin_complet);
                fichiers->taille++;
            }
        }

    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
    return fichiers;
}

#else

/* Verifie si un chemin est un dossier */
int est_dossier(const char *chemin) {
    struct stat path_stat;
    if (stat(chemin, &path_stat) != 0)
        return 0;
    return S_ISDIR(path_stat.st_mode);
}

/* Verifie si un fichier a l'extension donnee */
int est_extension(const char *nom, const char *ext) {
    const char *dot = strrchr(nom, '.');
    if (!dot || dot == nom)
        return 0;
    return strcmp(dot + 1, ext) == 0;
}

/*
 * Parcourt un dossier et ses sous-dossiers (POSIX).
 * Retourne les fichiers correspondant a l'extension donnee.
 */
Fichiers *renvoie_fichier_dossier(const char *chemin_dossier, const char *extension,
                                  Fichiers *fichiers) {
    /* Premier appel : allocation de la structure */
    if (fichiers == NULL) {
        fichiers = malloc_gestion_echec_compteur(sizeof(Fichiers));
        fichiers->taille = 0;
        fichiers->capacite = 10;
        fichiers->noms =
            malloc_gestion_echec_compteur(fichiers->capacite * sizeof(*fichiers->noms));
        fichiers->temps =
            malloc_gestion_echec_compteur(fichiers->capacite * sizeof(*fichiers->temps));
    }

    DIR *dir = opendir(chemin_dossier);
    if (!dir) {
        log_fmt(NiveauLogErreur, "Could not open directory: %s", chemin_dossier);
        return fichiers;
    }

    struct dirent *courant;
    while ((courant = readdir(dir)) != NULL) {
        /* Ignore . et .. */
        if (strcmp(courant->d_name, ".") == 0 || strcmp(courant->d_name, "..") == 0) {
            continue;
        }

        char chemin_complet[TAILLE_LIEN];
        int len = snprintf(chemin_complet, TAILLE_LIEN, "%s/%s", chemin_dossier, courant->d_name);

        if (len >= TAILLE_LIEN) {
            log_fmt(NiveauLogAvertissement, "Path too long: %s/%s", chemin_dossier,
                    courant->d_name);
            continue;
        }

        if (est_dossier(chemin_complet)) {
            /* Recursion dans les sous-dossiers */
            renvoie_fichier_dossier(chemin_complet, extension, fichiers);
        } else {
            if (est_extension(courant->d_name, extension)) {
                /* Agrandissement si necessaire */
                if (fichiers->taille >= fichiers->capacite) {
                    fichiers->capacite *= 2;
                    fichiers->noms = realloc_gestion_echec_compteur(
                        fichiers->noms, fichiers->capacite * sizeof(*fichiers->noms));
                    fichiers->temps = realloc_gestion_echec_compteur(
                        fichiers->temps, fichiers->capacite * sizeof(*fichiers->temps));
                }

                snprintf(fichiers->noms[fichiers->taille], TAILLE_LIEN, "%s", chemin_complet);
                fichiers->temps[fichiers->taille] = renvoie_temp_fichier(chemin_complet);
                fichiers->taille++;
            }
        }
    }

    closedir(dir);
    return fichiers;
}

#endif
