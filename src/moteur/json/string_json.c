#include "../../chiffrement/aes.h"
#include "../../main.h"
#include "cJSON.h"
#include "json.h"

/* ecrit une string sur le bon chemin json */
void ecrire_string_json(const char *fichier_nom, const char *nom_variable, const char *valeur) {
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

    if (item) {
        cJSON_ReplaceItemInObject(curseur, token, cJSON_CreateString(valeur));
    } else {
        cJSON_AddStringToObject(curseur, token, valeur);
    }
}

/*recupere une string sur le bon chemin json */
char *recuperer_string_json(const char *fichier_nom, const char *nom_variable) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu) {
        log_fmt(NiveauLogErreur, "Invalid files path : %s or file doesnt open", fichier_nom);
        return 0;
    }

    cJSON *cible = naviguer_vers_cible((cJSON *)f->contenu, nom_variable);

    if (cJSON_IsString(cible) && (cible->valuestring != NULL)) {
        return cible->valuestring;
    }

    log_fmt(NiveauLogErreur, "Path '%s' not found or not a string in file '%s'", nom_variable,
            fichier_nom);
    return NULL;
}