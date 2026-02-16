#include "../../chiffrement/aes.h"
#include "../../moteur/logging/logging.h"
#include "cJSON.h"
#include "json.h"

/* ecrit un int sur le bon chemin json */
void ecrire_int_json(const char *fichier_nom, const char *nom_variable, int valeur) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu) {
        log_fmt(NiveauLogErreur, "Invalid files path : %s or file doesnt open", fichier_nom);
        return;
    }

    char *token = NULL;

    cJSON *parent = naviguer_vers_cible((cJSON *)f->contenu, nom_variable, &token, true);

    if (!parent || !token) {
        log_fmt(NiveauLogErreur, "Path error '%s' invalid in %s", nom_variable, fichier_nom);
        return;
    }

    cJSON *item = cJSON_GetObjectItemCaseSensitive(parent, token);

    if (item && cJSON_IsNumber(item)) {
        cJSON_SetNumberValue(item, valeur);
    } else {
        if (item)
            cJSON_DeleteItemFromObjectCaseSensitive(parent, token);
        cJSON_AddNumberToObject(parent, token, valeur);
    }
}

int recuperer_int_json(const char *fichier_nom, const char *nom_variable) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu) {
        log_fmt(NiveauLogErreur, "Invalid files path : %s or file doesnt open", fichier_nom);
        return 0;
    }

    char *token = NULL;

    /* ne cree pas les parents */
    cJSON *parent = naviguer_vers_cible((cJSON *)f->contenu, nom_variable, &token, false);

    if (!parent || !token) {
        log_fmt(NiveauLogErreur, "Path '%s' invalid in %s", nom_variable, fichier_nom);
        return 0;
    }
    /* recupere lenfant */
    cJSON *cible = cJSON_GetObjectItemCaseSensitive(parent, token);

    if (cible && cJSON_IsNumber(cible))
        return cible->valueint;

    log_fmt(NiveauLogErreur, "Path '%s' invalid in %s", nom_variable, fichier_nom);
    return 0;
}
