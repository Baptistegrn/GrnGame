#include "../../chiffrement/aes.h"
#include "../../main.h"
#include "cJSON.h"
#include "json.h"

/* Verifie si la cle est nulle (pas de chiffrement) */
static bool est_cle_nulle() {
    if (!gs)
        return true;
    for (int i = 0; i < 16; i++) {
        if (gs->fichiers->cle[i] != 0)
            return false;
    }
    return true;
}

/* Realloue la liste des fichiers si pleine */
static void reallouer_liste_si_plein() {
    if (!gs)
        goto gsvide;

    GestionnaireFichiers *gf = gs->fichiers;

    if (gf->nb_fichiers >= gf->capacite) {

        FichierJSON *nouvelle_liste =
            realloc_gestion_echec_compteur(gf->liste, sizeof(FichierJSON) * gf->capacite * 2);
        gf->capacite *= 2;
    }
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in reallocation of json list");
}

/* Trouve un fichier deja charge dans la liste */
FichierJSON *trouver_fichier_dans_liste(const char *nom) {
    if (!gs)
        return NULL;
    for (int i = 0; i < gs->fichiers->nb_fichiers; i++) {
        if (strncmp(gs->fichiers->liste[i].nom, nom, TAILLE_LIEN) == 0) {
            return &gs->fichiers->liste[i];
        }
    }
    return NULL;
}

/* cryptage (liberation obligatoire)*/
static uint8_t *crypter_aes_interne(const char *texte, size_t *taille_sortie) {
    size_t taille_origine = strlen(texte);

    /* Calcul padding */
    uint8_t padding_val = 16 - (taille_origine % 16);
    size_t taille_chiffree = taille_origine + padding_val;

    uint8_t *buffer = malloc_gestion_echec_compteur(taille_chiffree);
    if (!buffer)
        return NULL;

    /* Copie du texte */
    memcpy(buffer, texte, taille_origine);

    /* Ajout du padding */
    for (size_t i = taille_origine; i < taille_chiffree; i++) {
        buffer[i] = padding_val;
    }

    /* Chiffrement AES-CBC */
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, gs->fichiers->cle, gs->fichiers->iv);
    AES_CBC_encrypt_buffer(&ctx, buffer, (uint32_t)taille_chiffree);

    *taille_sortie = taille_chiffree;
    return buffer;
}

/* Decryptage (liberation obligatoire) */
static char *decrypter_aes_interne(uint8_t *buffer, size_t taille_buffer) {
    if (!buffer || taille_buffer == 0 || taille_buffer % 16 != 0)
        return NULL;

    /* Decryptage en place */
    uint8_t *buffer_travail = malloc_gestion_echec_compteur(taille_buffer);
    if (!buffer_travail)
        return NULL;
    memcpy(buffer_travail, buffer, taille_buffer);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, gs->fichiers->cle, gs->fichiers->iv);
    AES_CBC_decrypt_buffer(&ctx, buffer_travail, (uint32_t)taille_buffer);

    /* Verification Padding PKCS7 */
    uint8_t padding_val = buffer_travail[taille_buffer - 1];
    if (padding_val < 1 || padding_val > 16) {
        free_gestion_echec_compteur(buffer_travail);
        return NULL; /* Padding invalide ou mauvaise cle */
    }

    size_t taille_reelle = taille_buffer - padding_val;

    /* Creation de la string */
    char *texte_final = malloc_gestion_echec_compteur(taille_reelle + 1);
    if (texte_final) {
        memcpy(texte_final, buffer_travail, taille_reelle);
        texte_final[taille_reelle] = '\0';
    }

    free_gestion_echec_compteur(buffer_travail);
    return texte_final;
}

/* Alloue et initialise le gestionnaire */
void creer_gestionnaire_fichiers(void) {
    if (!gs)
        goto gsvide;

    /* Allocation structure principale */
    gs->fichiers = malloc_gestion_echec_compteur(sizeof(GestionnaireFichiers));

    memset(gs->fichiers, 0, sizeof(GestionnaireFichiers));
    GestionnaireFichiers *fichiers = gs->fichiers;

    /* Allocation liste initiale */
    fichiers->capacite = 4;
    fichiers->nb_fichiers = 0;
    fichiers->liste = malloc_gestion_echec_compteur(sizeof(FichierJSON) * fichiers->capacite);
    memset(fichiers->liste, 0, sizeof(FichierJSON) * fichiers->capacite);

    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in creation of json file manager");
}

/* charge un fichier json dans le gestionnaire */
void charger_fichier_json(const char *fichier_nom) {
    if (!gs)
        return;

    /* eviter doublons */
    if (trouver_fichier_dans_liste(fichier_nom))
        return;
    /* reallocation automatique */
    reallouer_liste_si_plein();

    cJSON *json_root = NULL;
    FILE *fp = NULL;
    uint8_t *buffer_fichier = NULL;
    char *texte_clair = NULL;

    /* Mode lecture : binaire si chiffré, texte sinon */
    int chiffre = !est_cle_nulle();
    fp = fopen(fichier_nom, chiffre ? "rb" : "r");

    if (fp) {
        /* Recuperer taille */
        fseek(fp, 0, SEEK_END);
        long taille = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (taille > 0) {
            buffer_fichier = malloc_gestion_echec_compteur(taille + 1);
            if (buffer_fichier) {
                fread(buffer_fichier, 1, taille, fp);
                buffer_fichier[taille] = 0; /* Securite */

                if (chiffre) {
                    /* Decryptage */
                    texte_clair = decrypter_aes_interne(buffer_fichier, (size_t)taille);
                    if (!texte_clair) {
                        log_fmt(NiveauLogErreur, "Decryption failed for %s", fichier_nom);
                        /* On cree un objet vide pour eviter crash */
                        json_root = cJSON_CreateObject();
                    } else {
                        json_root = cJSON_Parse(texte_clair);
                        free_gestion_echec_compteur(texte_clair);
                    }
                } else {
                    /* Lecture directe texte */
                    json_root = cJSON_Parse((char *)buffer_fichier);
                }
                free_gestion_echec_compteur(buffer_fichier);
            }
        }
        fclose(fp);
    }

    /* Si fichier inexistant ou parse error, on cree un vide */
    if (!json_root) {
        if (fp)
            log_fmt(NiveauLogErreur, "JSON Parse error in %s", fichier_nom);
        json_root = cJSON_CreateObject();
    }

    /* Ajout a la liste */
    GestionnaireFichiers *gf = gs->fichiers;
    FichierJSON *f = &gf->liste[gf->nb_fichiers];

    strncpy(f->nom, fichier_nom, TAILLE_LIEN - 1);
    f->nom[TAILLE_LIEN - 1] = '\0';
    f->contenu = (void *)json_root;

    gf->nb_fichiers++;

    log_fmt(NiveauLogInfo, "JSON Loaded: %s", fichier_nom);
}

/* sauvegarde de buffer a memoire */
void sauvegarder_fichier_json(const char *fichier_nom) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu) {
        log_fmt(NiveauLogErreur, "Cannot save unloaded file: %s", fichier_nom);
        return;
    }

    /* Serialisation */
    char *texte_json = cJSON_Print((cJSON *)f->contenu);
    if (!texte_json)
        return;

    int chiffre = !est_cle_nulle();
    FILE *fp = fopen(fichier_nom, chiffre ? "wb" : "w");

    if (!fp) {
        log_fmt(NiveauLogErreur, "Unable to open file for writing: %s", fichier_nom);
        free(texte_json); /* cJSON utilise malloc standard */
        return;
    }

    if (chiffre) {
        size_t taille_enc;
        uint8_t *buffer_enc = crypter_aes_interne(texte_json, &taille_enc);
        if (buffer_enc) {
            fwrite(buffer_enc, 1, taille_enc, fp);
            free_gestion_echec_compteur(buffer_enc);
        }
    } else {
        fprintf(fp, "%s", texte_json);
    }

    fclose(fp);
    free(texte_json); /* cJSON utilise malloc standard */
}

/* supprime un fichier */
void supprimer_fichier_json(const char *fichier_nom) {
    /* Suppression Disque */
    remove(fichier_nom);

    /* Suppression dans la structure */
    if (!gs)
        return;
    GestionnaireFichiers *gf = gs->fichiers;

    for (int i = 0; i < gf->nb_fichiers; i++) {
        if (strncmp(gf->liste[i].nom, fichier_nom, TAILLE_LIEN) == 0) {
            /* On libere le contenu cJSON */
            if (gf->liste[i].contenu) {
                cJSON_Delete((cJSON *)gf->liste[i].contenu);
            }
            for (int j = i; j < gf->nb_fichiers - 1; j++) {
                gf->liste[j] = gf->liste[j + 1];
            }
            memset(&gf->liste[gf->nb_fichiers - 1], 0, sizeof(FichierJSON));
            gf->nb_fichiers--;
            return;
        }
    }
}

void supprimer_variable_json(const char *fichier_nom, const char *nom_variable) {
    FichierJSON *fichier = trouver_fichier_dans_liste(fichier_nom);
    if (!fichier || !fichier->contenu) {
        log_fmt(NiveauLogErreur, "File not loaded '%s'. Cannot delete variable.", fichier_nom);
        return;
    }

    cJSON *curseur = (cJSON *)fichier->contenu;
    char buffer[TAILLE_LIEN];
    strncpy(buffer, nom_variable, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, ".");
    char *suivant = strtok(NULL, ".");

    while (suivant != NULL) {
        curseur = cJSON_GetObjectItemCaseSensitive(curseur, token);
        if (!curseur) {
            log_fmt(NiveauLogErreur, "Path broken at '%s' in file: %s", token, fichier_nom);
            return;
        }

        token = suivant; // On avance
        suivant = strtok(NULL, ".");
    }

    if (cJSON_HasObjectItem(curseur, token)) {
        cJSON_DeleteItemFromObjectCaseSensitive(curseur, token);
        log_fmt(NiveauLogInfo, "Variable '%s' deleted.", token);
    } else {
        log_fmt(NiveauLogErreur, "Variable '%s' does not exist in file: %s", token, fichier_nom);
    }
}

/* recupere le type d'une variable dans le json */
int recuperer_type_json(const char *fichier_nom, const char *nom_variable) {
    FichierJSON *f = trouver_fichier_dans_liste(fichier_nom);
    if (!f || !f->contenu)
        return JSON_TYPE_INCONNU;

    cJSON *cible = naviguer_vers_cible((cJSON *)f->contenu, nom_variable);

    if (!cible)
        return JSON_TYPE_INCONNU;

    if (cJSON_IsNumber(cible))
        return JSON_TYPE_NOMBRE;
    if (cJSON_IsString(cible))
        return JSON_TYPE_STRING;
    return JSON_TYPE_INCONNU;
}

/* applique la cle de fichier a l'index donne */
void mettre_fichiers_cle(int index, int valeur) {
    if (gs && index >= 0 && index < 16) {
        gs->fichiers->cle[index] = valeur;
        /* ne pas dire la cle dans les logs */
        log_message(NiveauLogInfo, "crypted key changed");
    }
}

/* applique l'iv de fichier a l'index donne */
void mettre_fichiers_iv(int index, int valeur) {
    if (gs && index >= 0 && index < 16) {
        gs->fichiers->iv[index] = valeur;
        /* ne pas dire l'iv dans les logs */
        log_message(NiveauLogInfo, "iv changed");
    }
}