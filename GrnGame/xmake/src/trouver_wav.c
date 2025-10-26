#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#endif

int ends_with_wav(const char *name)
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

    const char *ext = name + len - 4;
    return (tolower((unsigned char)ext[0]) == '.' &&
            tolower((unsigned char)ext[1]) == 'w' &&
            tolower((unsigned char)ext[2]) == 'a' &&
            tolower((unsigned char)ext[3]) == 'v');
}

#ifdef _WIN32
int collect_wavs(const char *dir, char ***out_list, int *out_count)
{
    if (!dir || !out_list || !out_count)
    {
        fprintf(stderr, "Erreur: Paramètres NULL dans la collecte de fichier son .wav dans le dossier\n");
        return -1;
    }

    char search_path[PATH_MAX];
    snprintf(search_path, sizeof(search_path), "%s\\*", dir);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(search_path, &fd);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le dossier '%s': %lu\n",
                dir, GetLastError());
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
            if (collect_wavs(fullpath, out_list, out_count) != 0)
            {
                fprintf(stderr, "Attention: Échec de la collecte dans le sous-dossier '%s'\n", fullpath);
            }
        }

        else if (ends_with_wav(fd.cFileName))
        {
            char **tmp = realloc(*out_list, sizeof(char *) * (*out_count + 1));
            if (!tmp)
            {
                fprintf(stderr, "Erreur: Échec de réallocation mémoire lors de la collecte des WAV\n");
                FindClose(hFind);
                return -1;
            }

            *out_list = tmp;
            (*out_list)[*out_count] = _strdup(fullpath);

            if (!(*out_list)[*out_count])
            {
                fprintf(stderr, "Erreur: Échec de duplication de chaîne pour '%s'\n", fullpath);
                FindClose(hFind);
                return -1;
            }

            (*out_count)++;
        }
    } while (FindNextFile(hFind, &fd));

    FindClose(hFind);
    return 0;
}

#else

int collect_wavs(const char *dir, char ***out_list, int *out_count)
{
    if (!dir || !out_list || !out_count)
    {
        fprintf(stderr, "Erreur: Paramètres NULL dans la collecte de fichier son .wav dans le dossier\n");
        return -1;
    }

    DIR *dp = opendir(dir);
    if (!dp)
    {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le dossier '%s': %s\n",
                dir, strerror(errno));
        return -1;
    }

    struct dirent *entry;
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
            fprintf(stderr, "Attention: Impossible d'obtenir les informations de '%s': %s\n",
                    fullpath, strerror(errno));
            continue;
        }


        if (S_ISDIR(st.st_mode))
        {
            if (collect_wavs(fullpath, out_list, out_count) != 0)
            {
                fprintf(stderr, "Attention: Échec de la collecte dans le sous-dossier '%s'\n", fullpath);
            }
        }

        else if (ends_with_wav(entry->d_name))
        {
            char **tmp = realloc(*out_list, sizeof(char *) * (*out_count + 1));
            if (!tmp)
            {
                fprintf(stderr, "Erreur: Échec de réallocation mémoire lors de la collecte des WAV\n");
                closedir(dp);
                return -1;
            }

            *out_list = tmp;
            (*out_list)[*out_count] = strdup(fullpath);

            if (!(*out_list)[*out_count])
            {
                fprintf(stderr, "Erreur: Échec de duplication de chaîne pour '%s'\n", fullpath);
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