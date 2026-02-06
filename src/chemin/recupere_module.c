/*
 * Extraction du nom de module Lua a partir dun chemin.
 */

#include "../main.h"

/*
 * Extrait le nom du module a partir d'un chemin complet.
 * Exemple: /chemin/vers/module.lua -> module
 * Retourne une chaine allouee a liberer par l'appelant.
 */
const char *recupere_module(const char *chemin) {
    if (!chemin)
        return NULL;

    /* Trouve le dernier separateur */
    const char *nom_fichier = strrchr(chemin, '/');
    if (nom_fichier)
        nom_fichier++;
    else
        nom_fichier = chemin;

    /* Verifie l'extension .lua */
    const char *ext = strrchr(nom_fichier, '.');
    if (!ext || strcmp(ext, ".lua") != 0)
        return NULL;

    size_t len = ext - nom_fichier;

    char *nom_module = malloc_gestion_echec_compteur(len + 1);
    if (!nom_module)
        return NULL;

    strncpy(nom_module, nom_fichier, len);
    nom_module[len] = '\0';

    return nom_module;
}
