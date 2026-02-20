#include "../../chiffrement/aes.h"
#include "../../moteur/logging/logging.h"
#include "../../proprietes.h"
#include "cJSON.h"
#include "json.h"
#include <stdio.h>

/* extrait chaques chemin complet possible depuis le chemin donne */
void extraire_chemins_recursif(cJSON *noeud, const char *chemin_actuel, cJSON *tableau_resultat) {
    cJSON *curseur = noeud->child;

    while (curseur != NULL) {
        if (curseur->string) {
            char nouveau_chemin[TAILLE_LIEN_JSON];
            if (chemin_actuel[0] == '\0') {
                snprintf(nouveau_chemin, sizeof(nouveau_chemin), "%s", curseur->string);
            } else {
                snprintf(nouveau_chemin, sizeof(nouveau_chemin), "%s.%s", chemin_actuel,
                         curseur->string);
            }
            /* recursion */
            if (cJSON_IsObject(curseur)) {
                extraire_chemins_recursif(curseur, nouveau_chemin, tableau_resultat);
            }
            /* si valeur complet on sarrete ici */
            else {
                cJSON_AddItemToArray(tableau_resultat, cJSON_CreateString(nouveau_chemin));
            }
        }
        curseur = curseur->next;
    }
}

/* liste tout les chemins a partir dun chemin de base et dun nom de fichier */
cJSON *lister_tous_les_chemins(const char *fichier_nom, const char *chemin) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu)
        return NULL;

    cJSON *tableau = cJSON_CreateArray();

    /* si chemin vide */
    if (chemin == NULL || chemin[0] == '\0') {
        extraire_chemins_recursif((cJSON *)f->contenu, "", tableau);
    }
    /* si chemin non vide */
    else {
        char *token = NULL;
        cJSON *parent = naviguer_vers_cible((cJSON *)f->contenu, chemin, &token, false);
        if (parent && token) {
            cJSON *cible = cJSON_GetObjectItemCaseSensitive(parent, token);

            if (cible && cJSON_IsObject(cible)) {
                extraire_chemins_recursif(cible, "", tableau);
            }
        }
    }

    return tableau;
}