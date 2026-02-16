#include "../../chiffrement/aes.h"
#include "../../moteur/logging/logging.h"
#include "cJSON.h"
#include "json.h"

/* ecrit une string sur le bon chemin json */
void ecrire_string_json(const char *fichier_nom, const char *nom_variable, const char *valeur) {
    FichierJSON *fichier = trouver_fichier_dans_liste(fichier_nom);
    if (!fichier || !fichier->contenu) {
        log_fmt(NiveauLogErreur, "Invalid files path : %s or file doesnt open", fichier_nom);
        return;
    }

    char *token = NULL;
    cJSON *parent = naviguer_vers_cible((cJSON *)fichier->contenu, nom_variable, &token, true);
    if (!parent || !token) {
        log_fmt(NiveauLogErreur, "Path error '%s' invalid in %s", nom_variable, fichier_nom);
        return;
    }

    cJSON *item = cJSON_GetObjectItemCaseSensitive(parent, token);

    if (item) {
        cJSON_ReplaceItemInObject(parent, token, cJSON_CreateString(valeur));
    } else {
        cJSON_AddStringToObject(parent, token, valeur);
    }
}

/* recupere une string sur le bon chemin json */
char *recuperer_string_json(const char *fichier_nom, const char *nom_variable) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu) {
        log_fmt(NiveauLogErreur, "Invalid files path : %s or file doesnt open", fichier_nom);
        return NULL;
    }

    char *token = NULL;
    cJSON *parent = naviguer_vers_cible((cJSON *)f->contenu, nom_variable, &token, false);
    if (!parent || !token) {
        log_fmt(NiveauLogErreur, "Path '%s' invalid in %s", nom_variable, fichier_nom);
        return NULL;
    }

    cJSON *cible = cJSON_GetObjectItemCaseSensitive(parent, token);
    if (cible && cJSON_IsString(cible) && cible->valuestring != NULL) {
        return cible->valuestring;
    }

    log_fmt(NiveauLogErreur, "Path '%s' not found or not a string in file '%s'", nom_variable,
            fichier_nom);
    return NULL;
}
