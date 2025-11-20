#include "main.h"
#include "logging.h" // Ajout pour log_fmt

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

int ends_with_png(const char* name)
{
    if (!name)
    {
        return 0;
    }

    size_t len = strlen(name);
    if (len < 4)
    {
        return 0;
    }

    const char* ext = name + len - 4;
    return (tolower(ext[0]) == '.' && tolower(ext[1]) == 'p' && tolower(ext[2]) == 'n' && tolower(ext[3]) == 'g');
}

#ifdef _WIN32
int collect_pngs(const char* dir, char*** out_list, int* out_count)
{
    if (!dir || !out_list || !out_count)
    {
        log_fmt(NiveauLogErreur, "Erreur: Paramètres NULL dans la collecte de fichier image .png dans le dossier");
        return -1;
    }

    char search_path[PATH_MAX];
    snprintf(search_path, sizeof(search_path), "%s\\*", dir);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(search_path, &fd);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'ouvrir le dossier '%s': %lu", dir, GetLastError());
        return -1;
    }

    do
    {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
        {
            continue;
        }

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s\\%s", dir, fd.cFileName);
        normaliser_chemin(fullpath);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (collect_pngs(fullpath, out_list, out_count) != 0)
            {
                log_fmt(NiveauLogAvertissement, "Attention: Échec de la collecte dans le sous-dossier '%s'", fullpath);
            }
        }

        else if (ends_with_png(fd.cFileName))
        {
            char** tmp = realloc(*out_list, sizeof(char*) * (*out_count + 1));
            if (!tmp)
            {
                log_fmt(NiveauLogErreur, "Erreur: Échec de réallocation mémoire lors de la collecte des PNG");
                FindClose(hFind);
                return -1;
            }

            *out_list = tmp;
            (*out_list)[*out_count] = _strdup(fullpath);

            if (!(*out_list)[*out_count])
            {
                log_fmt(NiveauLogErreur, "Erreur: Échec de duplication de chaîne pour '%s'", fullpath);
                FindClose(hFind);
                return -1;
            }

            (*out_count)++;
        }
    }
    while (FindNextFile(hFind, &fd));

    FindClose(hFind);
    return 0;
}

#else

int collect_pngs(const char* dir, char*** out_list, int* out_count)
{
    if (!dir || !out_list || !out_count)
    {
        log_fmt(NiveauLogErreur, "Erreur: Paramètres NULL dans la collecte de fichier image .png dans le dossier");
        return -1;
    }

    DIR* dp = opendir(dir);
    if (!dp)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible d'ouvrir le dossier '%s': %s", dir, strerror(errno));
        return -1;
    }

    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1)
        {
            log_fmt(NiveauLogAvertissement, "Attention: Impossible d'obtenir les informations de '%s': %s", fullpath,
                    strerror(errno));
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            if (collect_pngs(fullpath, out_list, out_count) != 0)
            {
                log_fmt(NiveauLogAvertissement, "Attention: Échec de la collecte dans le sous-dossier '%s'", fullpath);
            }
        }
        else if (ends_with_png(entry->d_name))
        {
            char** tmp = realloc(*out_list, sizeof(char*) * (*out_count + 1));
            if (!tmp)
            {
                log_fmt(NiveauLogErreur, "Erreur: Échec de réallocation mémoire lors de la collecte des PNG");
                closedir(dp);
                return -1;
            }

            *out_list = tmp;
            (*out_list)[*out_count] = strdup(fullpath);

            if (!(*out_list)[*out_count])
            {
                log_fmt(NiveauLogErreur, "Erreur: Échec de duplication de chaîne pour '%s'", fullpath);
                closedir(dp);
                return -1;
            }

            (*out_count)++;
        }
    }

    closedir(dp);
    return 0;
}
#endif