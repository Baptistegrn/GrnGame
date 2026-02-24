/*
 * Chargement des fichiers audio.
 * Gere le chargement des fichiers WAV et la recuperation par lien.
 */

#include "../../allouer/allouer.h"
#include "../../chemin/chemin.h"
#include "../../chiffrement/aes.h"
#include "../../main.h"
#include "../../prediction_branche.h"
#include "../logging/logging.h"
#include "son.h"
#include <stdio.h>
#include <string.h>

/* Verifie si le tableau de sons est plein et l'agrandit si necessaire */
void agrandir_si_plein_son(void) {
    if (UNLIKELY(!gs))
        goto gsvide;
    GestionnaireSon *sons = gs->sons;

    if (sons->taille >= sons->capacite) {
        int nouvelle_capacite = sons->capacite + 50;
        sons->entrees =
            realloc_gestion_echec_compteur(sons->entrees, sizeof(SonEntry) * nouvelle_capacite);

        /* Initialisation de la nouvelle zone a 0 */
        memset(sons->entrees + sons->capacite, 0, sizeof(SonEntry) * 50);

        sons->capacite = nouvelle_capacite;
    }
    return;

gsvide:
    log_message(NiveauLogErreur, "manager is empty during sound manager reallocation function");
}

/* Charge un fichier son (WAV) en memoire (dechiffre au vol si une cle est presente) */
Mix_Chunk *charger_un_son(const char *lien_complet) {
    if (UNLIKELY(!gs))
        goto gsvide;
    if (UNLIKELY(!lien_complet))
        return NULL;

    GestionnaireSon *sons = gs->sons;

    const uint8_t *cle = gs->sons->cle;

    char lien_norm[TAILLE_LIEN];
    normaliser_chemin_copies(lien_norm, lien_complet);

    /* On verifie si le son est deja dans le cache */
    Mix_Chunk *existant = recuperer_son_par_lien(lien_norm);
    if (existant)
        return existant;

    Mix_Chunk *son = NULL;

    /* Verification cle chiffre */
    bool chiffre = false;
    for (int i = 0; i < 16; i++) {
        if (cle[i] != 0) {
            chiffre = true;
            break;
        }
    }

    if (!chiffre) {
        /* Chargement standard */
        son = Mix_LoadWAV(lien_norm);
    } else {
        /* Chargement chiffrement */
        FILE *fs = fopen(lien_norm, "rb");
        if (!fs)
            return NULL;

        fseek(fs, 0, SEEK_END);
        long taille_fichier = ftell(fs);
        fseek(fs, 0, SEEK_SET);

        /* Le fichier doit etre un multiple de 16 octets */
        if (taille_fichier % 16 != 0 || taille_fichier == 0) {
            log_fmt(NiveauLogErreur, "Fichier son corrompu ou mal chiffre (taille %% 16 != 0): %s",
                    lien_norm);
            fclose(fs);
            return NULL;
        }
        /* buffer */
        uint8_t *buffer = malloc_gestion_echec_compteur(taille_fichier);
        if (buffer) {
            fread(buffer, 1, taille_fichier, fs);
            fclose(fs);

            /* decryptage */
            struct AES_ctx ctx;
            AES_init_ctx_iv(&ctx, cle, gs->sons->iv);
            AES_CBC_decrypt_buffer(&ctx, buffer, (size_t)taille_fichier);

            /* Suppression du padding */
            uint8_t padding_valeur = buffer[taille_fichier - 1];
            size_t taille_reelle = taille_fichier;

            if (padding_valeur >= 1 && padding_valeur <= 16) {
                taille_reelle = taille_fichier - padding_valeur;
            }

            /* Creation du son via le buffer dechiffre */
            SDL_RWops *rw = SDL_RWFromMem(buffer, (int)taille_reelle);
            if (rw) {
                son = Mix_LoadWAV_RW(rw, 1);
            }
            free_gestion_echec_compteur(buffer);
        } else {
            fclose(fs);
        }
    }

    if (UNLIKELY(!son)) {
        log_fmt(NiveauLogErreur, "Impossible to load '%s': %s", lien_norm, Mix_GetError());
        return NULL;
    }

    agrandir_si_plein_son();

    if (sons->taille >= sons->capacite) {
        log_message(NiveauLogErreur, "Sound manager capacity exceeded");
        Mix_FreeChunk(son);
        return NULL;
    }

    int index = sons->taille++;
    SonEntry *entree = &sons->entrees[index];

    strncpy(entree->id, lien_norm, TAILLE_LIEN - 1);
    entree->id[TAILLE_LIEN - 1] = '\0';
    entree->son = son;

    return son;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in load one sound function");
    return NULL;
}

/* Recupere un pointeur vers un son deja charge */
Mix_Chunk *recuperer_son_par_lien(const char *lien) {
    if (UNLIKELY(!gs))
        goto gsvide;
    if (UNLIKELY(!lien))
        return NULL;

    /* Copie locale pour recherche normalisee */
    char lien_recherche[TAILLE_LIEN];
    normaliser_chemin_copies(lien_recherche, lien);

    GestionnaireSon *sons = gs->sons;

    for (int i = 0; i < sons->taille; i++) {
        SonEntry *entree = &sons->entrees[i];
        if (strcmp(entree->id, lien_recherche) == 0) {
            return entree->son;
        }
    }

    return NULL;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in retrieve sound by link function");
    return NULL;
}

/* Scanne un dossier et charge tous les fichiers (WAV ou DATA si chiffre) */
void charger_tout_les_sons(const char *dossier) {
    if (UNLIKELY(!gs))
        goto gsvide;
    if (UNLIKELY(!dossier)) {
        log_message(NiveauLogErreur, "NULL directory parameter");
        return;
    }

    /* On regarde si on cherche du clair ou du chiffre */
    bool cle_est_nulle = true;
    for (int i = 0; i < 16; i++) {
        if (gs->sons->cle[i] != 0) {
            cle_est_nulle = false;
            break;
        }
    }

    Fichiers *fichiers = renvoie_fichier_dossier(NULL, "", NULL);

    if (cle_est_nulle) {
        fichiers = renvoie_fichier_dossier(dossier, "wav", fichiers);
    } else {
        fichiers = renvoie_fichier_dossier(dossier, "data", fichiers);
    }

    if (!fichiers)
        return;

    int nb = fichiers->taille;
    if (nb == 0) {
        liberer_fichiers(fichiers);
        return;
    }

    int nb_echecs = 0;

    /* Boucle sur les fichiers trouves */
    for (int i = 0; i < nb; i++) {
        Mix_Chunk *son = charger_un_son(fichiers->noms[i]);

        if (!son) {
            nb_echecs++;
        } else {
            log_fmt(NiveauLogDebug, "Sound loaded: %s", fichiers->noms[i]);
        }
    }

    liberer_fichiers(fichiers);

    if (nb_echecs > 0) {
        log_fmt(NiveauLogAvertissement, "%d sounds failed to load out of %d", nb_echecs, nb);
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in load all sounds function");
}

/* applique la cle de son a l'index donne */
void mettre_son_cle(int index, int valeur) {
    if (gs && index >= 0 && index < 16) {
        gs->sons->cle[index] = valeur;
        /* ne pas dire la cle dans les logs */
        log_message(NiveauLogInfo, "crypted key changed");
    }
}

/* applique l'iv de son a l'index donne */
void mettre_son_iv(int index, int valeur) {
    if (gs && index >= 0 && index < 16) {
        gs->sons->iv[index] = valeur;
        /* ne pas dire l'iv dans les logs */
        log_message(NiveauLogInfo, "iv changed");
    }
}