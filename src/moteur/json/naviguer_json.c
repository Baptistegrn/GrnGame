#include "../../chiffrement/aes.h"
#include "../../proprietes.h"
#include "cJSON.h"
#include <stdbool.h>
#include <string.h>

/* renvoie lobject cJSON PARENT au bout du chemin retourne dans le ptr la chaine de caractere enfant
 * et le booleen cree ou pas les parents */
cJSON *naviguer_vers_cible(cJSON *root, const char *chemin, char **ptr, bool creer_parent) {
    if (!root || !chemin)
        return NULL;

    char buffer[TAILLE_LIEN_JSON];
    strncpy(buffer, chemin, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    cJSON *curseur = root;
    char *token = strtok(buffer, ".");
    char *suivant = strtok(NULL, ".");
    char *dernier = token;

    while (suivant) {
        cJSON *enfant = cJSON_GetObjectItemCaseSensitive(curseur, token);

        if (!enfant) {
            if (!creer_parent)
                return NULL;

            enfant = cJSON_CreateObject();
            cJSON_AddItemToObject(curseur, token, enfant);
        }

        curseur = enfant;
        token = suivant;
        dernier = token;
        suivant = strtok(NULL, ".");
    }

    if (ptr)
        *ptr = dernier;

    return curseur;
}
