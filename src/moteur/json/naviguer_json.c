#include "../../chiffrement/aes.h"
#include "../../main.h"
#include "cJSON.h"

/* renvoie lobject cJSON correct selon le chemin */
cJSON *naviguer_vers_cible(cJSON *root, const char *chemin) {
    if (!root || !chemin)
        return NULL;

    char buffer[TAILLE_LIEN];
    strncpy(buffer, chemin, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    cJSON *curseur = root;
    char *token = strtok(buffer, ".");
    while (token != NULL) {
        curseur = cJSON_GetObjectItemCaseSensitive(curseur, token);
        if (!curseur)
            return NULL;
        token = strtok(NULL, ".");
    }
    return curseur;
}