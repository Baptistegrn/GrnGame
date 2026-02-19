/*
 * Chargement et gestion du cache de textures.
 */

#include "chargement_image.h"
#include "../../../allouer/allouer.h"
#include "../../../chemin/chemin.h"
#include "../../../chiffrement/aes.h"
#include "../../../main.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Verifie si le tableau est plein et l'agrandit */
void agrandir_si_plein() {
    if (!gs)
        goto gsvide;
    GestionnaireTextures *gt = gs->textures;

    if (gt->taille >= gt->capacite) {
        int nouvelle_capacite = gt->capacite + 50;
        gt->entrees =
            realloc_gestion_echec_compteur(gt->entrees, sizeof(TextureEntry) * nouvelle_capacite);
        memset(gt->entrees + gt->capacite, 0, sizeof(TextureEntry) * 50);
        gt->capacite = nouvelle_capacite;
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in enlarge texture manager");
}

SDL_Texture *charger_une_texture(const char *lien_complet) {
    if (!gs || !lien_complet)
        return NULL;

    GestionnaireTextures *gt = gs->textures;

    char lien_norm[TAILLE_LIEN];
    normaliser_chemin_copies(lien_norm, lien_complet);

    /* texture existante */
    SDL_Texture *existant = recuperer_texture_par_lien(lien_norm);
    if (existant)
        return existant;

    SDL_Surface *surface = NULL;

    /* verification cle chiffre */
    bool chiffre = false;
    for (int i = 0; i < 16; i++) {
        if (gt->cle[i] != 0) {
            chiffre = true;
            break;
        }
    }
    /* si pas chiffre on charge direct */
    if (!chiffre) {
        surface = IMG_Load(lien_norm);
    } else {
        FILE *fs = fopen(lien_norm, "rb");
        if (!fs)
            return NULL;
        /* taille */
        fseek(fs, 0, SEEK_END);
        long taille_fichier = ftell(fs);
        fseek(fs, 0, SEEK_SET);

        /* si le fichier a ete mal chiffre il nest pas modulo 16 */
        if (taille_fichier % 16 != 0 || taille_fichier == 0) {
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
            AES_init_ctx_iv(&ctx, gt->cle, gt->iv);
            AES_CBC_decrypt_buffer(&ctx, buffer, (size_t)taille_fichier);

            /* pas en plus a enlever */
            uint8_t padding_valeur = buffer[taille_fichier - 1];
            size_t taille_reelle = taille_fichier;

            if (padding_valeur >= 1 && padding_valeur <= 16) {
                taille_reelle = taille_fichier - padding_valeur;
            }
            /* chargement sdl */
            SDL_RWops *rw = SDL_RWFromMem(buffer, (int)taille_reelle);
            if (rw) {
                surface = IMG_Load_RW(rw, 1);
            }
            free_gestion_echec_compteur(buffer);
        } else {
            fclose(fs);
        }
    }

    if (!surface) {
        log_fmt(NiveauLogErreur, "Echec chargement %s: %s", lien_norm, IMG_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gs->fenetre->rendu, surface);
    SDL_FreeSurface(surface);

    if (tex) {
        agrandir_si_plein();
        int index = gt->taille++;
        TextureEntry *entree = &gt->entrees[index];
        strncpy(entree->id, lien_norm, TAILLE_LIEN - 1);
        entree->textures = creer_liste(tex, 0);
    }

    return tex;
}

/* Scanne un dossier et charge toutes les textures */
void charger_toutes_les_textures(const char *dossier) {
    if (!gs)
        goto gsvide;

    if (!dossier)
        return;
    const uint8_t *cle_de_chiffrement = gs->textures->cle;
    /* cle ou pas ?*/
    bool cle_est_nulle = true;
    for (int i = 0; i < 16; i++) {
        if (cle_de_chiffrement[i] != 0) {
            cle_est_nulle = false;
            break;
        }
    }
    /* pour charger la structure */
    Fichiers *fichiers = renvoie_fichier_dossier(NULL, "", NULL);
    /* on charge normalement en png, sinon en .data si chiffré */
    if (cle_est_nulle) {
        fichiers = renvoie_fichier_dossier(dossier, "png", fichiers);
    } else {
        fichiers = renvoie_fichier_dossier(dossier, "data", fichiers);
    }
    int nb = fichiers->taille;
    if (nb == 0) {
        liberer_fichiers(fichiers);
        return;
    }

    for (int i = 0; i < nb; i++) {
        SDL_Texture *tex = charger_une_texture(fichiers->noms[i]);
    }

    liberer_fichiers(fichiers);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in load all textures");
}

/* Recupere la texture angle 0 */
SDL_Texture *recuperer_texture_par_lien(const char *lien) {
    if (!gs)
        goto gsvide;
    if (!lien)
        return NULL;

    char lien_recherche[TAILLE_LIEN];
    normaliser_chemin_copies(lien_recherche, lien);

    GestionnaireTextures *gt = gs->textures;
    for (int i = 0; i < gt->taille; i++) {
        TextureEntry *entree = &gt->entrees[i];
        if (strcmp(entree->id, lien_recherche) == 0) {
            return recuperer_texture_angle_indice(entree->textures, 0);
        }
    }

    return NULL;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in retrieve texture by link");
    return NULL;
}

/* Ajoute une variante de texture au cache */
void ajouter_variante_cache(const char *id, SDL_Texture *nouvelle_tex, int angle) {
    if (!gs)
        goto gsvide;
    if (!id || !nouvelle_tex)
        return;

    /* Copie locale pour recherche */
    char lien_recherche[TAILLE_LIEN];
    normaliser_chemin_copies(lien_recherche, id);

    GestionnaireTextures *gt = gs->textures;

    for (int i = 0; i < gt->taille; i++) {
        TextureEntry *entree = &gt->entrees[i];

        if (strcmp(entree->id, lien_recherche) == 0) {
            if (recuperer_texture_angle_indice(entree->textures, angle) == NULL) {
                entree->textures = ajouter_en_tete(entree->textures, nouvelle_tex, angle);
            }
            return;
        }
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add a rotated form in the cache");
}

/* Recupere une variante de texture selon l'angle */
SDL_Texture *recuperer_texture_variante(const char *lien, int angle) {
    if (!gs)
        goto gsvide;
    if (!lien)
        return NULL;

    char lien_recherche[TAILLE_LIEN];
    normaliser_chemin_copies(lien_recherche, lien);
    GestionnaireTextures *gt = gs->textures;
    for (int i = 0; i < gt->taille; i++) {
        TextureEntry *entree = &gt->entrees[i];
        if (strcmp(entree->id, lien_recherche) == 0) {
            return recuperer_texture_angle_indice(entree->textures, angle);
        }
    }

    return NULL;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in retrieve rotated texture from cache");
    return NULL;
}

/* applique la cle d'image a l'index donne */
void mettre_image_cle(int index, int valeur) {
    if (gs && index >= 0 && index < 16) {
        gs->textures->cle[index] = valeur;
        /* ne pas dire la cle dans les logs */
        log_message(NiveauLogInfo, "crypted key changed");
    }
}

/* applique l'iv d'image a l'index donne */
void mettre_image_iv(int index, int valeur) {
    if (gs && index >= 0 && index < 16) {
        gs->textures->iv[index] = valeur;
        /* ne pas dire l'iv dans les logs */
        log_message(NiveauLogInfo, "iv changed");
    }
}