#include "../../chiffrement/aes.h"
#include "../../main.h"
#include "cJSON.h"
#include "json.h"

/* ecrit un int sur le bon chemin json */
void ecrire_int_json(const char *fichier_nom, const char *nom_variable, int valeur) {
    FichierJSON *fichier = trouver_fichier_dans_liste(fichier_nom);
    if (!fichier || !fichier->contenu)
        return;

    cJSON *curseur = (cJSON *)fichier->contenu;
    char buffer[TAILLE_LIEN];
    strncpy(buffer, nom_variable, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *dernier_token = NULL;
    char *token = strtok(buffer, ".");

    while (token != NULL) {
        dernier_token = token;
        char *token_suivant = strtok(NULL, ".");
        /* sarreter ici si on arrive au bout */
        if (token_suivant == NULL) {
            break;
        }

        cJSON *suivant =
            cJSON_GetObjectItemCaseSensitive(curseur, token_suivant ? token : token_suivant);

        cJSON *sub = cJSON_GetObjectItemCaseSensitive(curseur, token);
        if (!sub) {
            sub = cJSON_CreateObject();
            cJSON_AddItemToObject(curseur, token, sub);
        }
        curseur = sub;
        token = token_suivant;
    }

    cJSON *item = cJSON_GetObjectItemCaseSensitive(curseur, token);
    if (item && cJSON_IsNumber(item)) {
        cJSON_SetNumberValue(item, valeur);
    } else {
        if (item)
            cJSON_DeleteItemFromObject(curseur, token);
        cJSON_AddNumberToObject(curseur, token, valeur);
    }
}

/* Recupere un entier dans le json (0 par defaut) */
int recuperer_int_json(const char *fichier_nom, const char *nom_variable) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu) {
        log_fmt(NiveauLogErreur, "Invalid files path : %s or file doesnt open", fichier_nom);
        return 0;
    }

    cJSON *cible = naviguer_vers_cible((cJSON *)f->contenu, nom_variable);

    if (cible && cJSON_IsNumber(cible)) {
        return cible->valueint;
    }

    log_fmt(NiveauLogErreur, "Path '%s' invalid in %s", nom_variable, fichier_nom);
    return 0;
}