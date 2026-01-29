#include "../main.h"

#ifdef _WIN32
int est_extension(const char *nom, const char *ext) {
    const char *dot = strrchr(nom, '.');
    if (!dot || dot == nom)
        return 0;
    return strcmp(dot + 1, ext) == 0;
}

Fichiers *renvoie_fichier_dossier(const char *chemin_dossier, const char *extension,
                                  Fichiers *fichiers) {
    /* cas premier appel : allocation */
    if (fichiers == NULL) {
        fichiers = xmalloc(sizeof(Fichiers));
        fichiers->taille = 0;
        fichiers->capacite = 10;
        fichiers->noms = xmalloc(fichiers->capacite * sizeof(*fichiers->noms));
        fichiers->temps = xmalloc(fichiers->capacite * sizeof(*fichiers->temps));
    }
    char chemin_recherche[TAILLE_LIEN_GT];
    snprintf(chemin_recherche, TAILLE_LIEN_GT, "%s/*", chemin_dossier);

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(chemin_recherche, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        log_fmt(NiveauLogErreur, "Could not open directory (Win32): %s", chemin_dossier);
        return fichiers;
    }

    do {
        /* on ignore . et .. */
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
            continue;
        }
        char chemin_complet[TAILLE_LIEN_GT];
        int len =
            snprintf(chemin_complet, TAILLE_LIEN_GT, "%s/%s", chemin_dossier, findData.cFileName);

        if (len >= TAILLE_LIEN_GT) {
            log_fmt(NiveauLogAvertissement, "Path too long: %s/%s", chemin_dossier,
                    findData.cFileName);
            continue;
        }

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            /*recursion*/
            renvoie_fichier_dossier(chemin_complet, extension, fichiers);
        } else {
            if (est_extension(findData.cFileName, extension)) {
                if (fichiers->taille >= fichiers->capacite) {
                    fichiers->capacite *= 2;
                    fichiers->noms =
                        xrealloc(fichiers->noms, fichiers->capacite * sizeof(*fichiers->noms));
                    fichiers->temps =
                        xrealloc(fichiers->temps, fichiers->capacite * sizeof(*fichiers->temps));
                }
                snprintf(fichiers->noms[fichiers->taille], TAILLE_LIEN_GT, "%s", chemin_complet);
                fichiers->temps[fichiers->taille] = renvoie_temp_fichier(chemin_complet);
                fichiers->taille++;
            }
        }

    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
    return fichiers;
}
#else
int est_dossier(const char *chemin) {
    struct stat path_stat;
    if (stat(chemin, &path_stat) != 0)
        return 0;
    return S_ISDIR(path_stat.st_mode);
}

int est_extension(const char *nom, const char *ext) {
    const char *dot = strrchr(nom, '.');
    if (!dot || dot == nom)
        return 0;
    return strcmp(dot + 1, ext) == 0;
}

Fichiers *renvoie_fichier_dossier(const char *chemin_dossier, const char *extension,
                                  Fichiers *fichiers) {
    /* cas premier appel */
    if (fichiers == NULL) {
        fichiers = xmalloc(sizeof(Fichiers));
        fichiers->taille = 0;
        fichiers->capacite = 10;
        fichiers->noms = xmalloc(fichiers->capacite * sizeof(*fichiers->noms));
        fichiers->temps = xmalloc(fichiers->capacite * sizeof(*fichiers->temps));
    }

    DIR *dir = opendir(chemin_dossier);
    if (!dir) {
        log_fmt(NiveauLogErreur, "Could not open directory: %s", chemin_dossier);
        return fichiers;
    }

    struct dirent *courant;
    while ((courant = readdir(dir)) != NULL) {
        /* on ignore . et .. */
        if (strcmp(courant->d_name, ".") == 0 || strcmp(courant->d_name, "..") == 0) {
            continue;
        }
        /* chemin complet */
        char chemin_complet[TAILLE_LIEN_GT];
        int len =
            snprintf(chemin_complet, TAILLE_LIEN_GT, "%s/%s", chemin_dossier, courant->d_name);

        if (len >= TAILLE_LIEN_GT) {
            log_fmt(NiveauLogAvertissement, "you give too long path: %s/%s", chemin_dossier,
                    courant->d_name);
            continue;
        }

        /* recursion */
        if (est_dossier(chemin_complet)) {
            renvoie_fichier_dossier(chemin_complet, extension, fichiers);
        } else {
            if (est_extension(courant->d_name, extension)) {
                if (fichiers->taille >= fichiers->capacite) {
                    fichiers->capacite *= 2;
                    fichiers->noms =
                        xrealloc(fichiers->noms, fichiers->capacite * sizeof(*fichiers->noms));
                    fichiers->temps =
                        xrealloc(fichiers->temps, fichiers->capacite * sizeof(*fichiers->temps));
                }

                snprintf(fichiers->noms[fichiers->taille], TAILLE_LIEN_GT, "%s", chemin_complet);
                fichiers->temps[fichiers->taille] = renvoie_temp_fichier(chemin_complet);
                fichiers->taille++;
            }
        }
    }

    closedir(dir);
    return fichiers;
}

#endif
