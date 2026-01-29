#include "../main.h"

/*recupere le nom du module lua a partir du chemin complet
 * ex: /chemin/vers/module.lua -> module
 * a liberer par l'appelant
 */
const char *recupere_module(const char *chemin) {
    if (!chemin)
        return NULL;
    // decoupe pour avoir le dernier /
    const char *nom_fichier = strrchr(chemin, '/');
    if (nom_fichier)
        nom_fichier++;
    else
        nom_fichier = chemin;

    // Trouve l'extension
    const char *ext = strrchr(nom_fichier, '.');
    if (!ext || strcmp(ext, ".lua") != 0)
        return NULL;

    size_t len = ext - nom_fichier;

    char *nom_module = xmalloc(len + 1);
    if (!nom_module)
        return NULL;

    strncpy(nom_module, nom_fichier, len);
    nom_module[len] = '\0';

    return nom_module;
}
