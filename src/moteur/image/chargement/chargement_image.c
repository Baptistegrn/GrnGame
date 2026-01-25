#include "../../../main.h"

/*
 * Initialise le sous-système de textures.
 */
void init_gestionnaire_textures() {
    if (!gs)
        goto gsvide;
    GestionnaireTextures *gt = gs->textures;

    gt->capacite = 50;
    gt->taille = 0;

    gt->entrees = xmalloc(gt->capacite * sizeof(TextureEntry));
    memset(gt->entrees, 0, gt->capacite * sizeof(TextureEntry));
    return;

gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in init texture");
}

/*
 * Vérifie si le tableau est plein et l'agrandit.
 */
void agrandir_si_plein() {
    if (!gs)
        goto gsvide;
    GestionnaireTextures *gt = gs->textures;

    if (gt->taille >= gt->capacite) {
        int nouvelle_capacite = gt->capacite + 50;
        gt->entrees = xrealloc(gt->entrees, sizeof(TextureEntry) * nouvelle_capacite);
        memset(gt->entrees + gt->capacite, 0, sizeof(TextureEntry) * 50);
        gt->capacite = nouvelle_capacite;
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in enlarge texture manager");
}

/*
 * Charge une texture unique.
 */
SDL_Texture *charger_une_texture(const char *lien_complet) {
    if (!gs)
        goto gsvide;
    if (!lien_complet)
        return NULL;

    GestionnaireTextures *gt = gs->textures;

    char lien_norm[TAILLE_LIEN_GT];
    normaliser_chemin_copies(lien_norm, lien_complet);

    SDL_Texture *existant = recuperer_texture_par_lien(lien_norm);
    if (existant)
        return existant;

    /* 3. Chargement (SDL accepte les / sous Windows) */
    SDL_Surface *surface = IMG_Load(lien_norm);
    if (!surface) {
        log_fmt(NiveauLogErreur, "Img not found '%s': %s", lien_norm, IMG_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gs->fenetre->rendu, surface);
    SDL_FreeSurface(surface);

    if (!tex)
        return NULL;

    agrandir_si_plein();

    if (gt->taille >= gt->capacite) {
        SDL_DestroyTexture(tex);
        return NULL;
    }

    int index = gt->taille++;
    TextureEntry *entree = &gt->entrees[index];

    strncpy(entree->id, lien_norm, TAILLE_LIEN_GT - 1);
    entree->id[TAILLE_LIEN_GT - 1] = '\0';

    entree->textures = creer_liste(tex, 0);

    return tex;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in loading a texture");
    return NULL;
}

/*
 * Scanne un dossier et charge tout.
 */
void charger_toutes_les_textures(const char *dossier) {
    if (!gs)
        goto gsvide;
    if (!dossier)
        return;

    Fichiers *fichiers = renvoie_fichier_dossier(dossier, "png", NULL);
    if (!fichiers)
        return;

    int nb = fichiers->taille;
    if (nb == 0) {
        liberer_fichiers(fichiers);
        return;
    }

    for (int i = 0; i < nb; i++) {
        SDL_Texture *tex = charger_une_texture(fichiers->noms[i]);
        if (tex) {
            log_fmt(NiveauLogDebug, "Texture loaded: %s", fichiers->noms[i]);
        }
    }

    liberer_fichiers(fichiers);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in load all textures");
}

/*
 * Récupère la texture angle 0.
 */
SDL_Texture *recuperer_texture_par_lien(const char *lien) {
    if (!gs)
        goto gsvide;
    if (!lien)
        return NULL;

    char lien_recherche[TAILLE_LIEN_GT];
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

/*
 * Ajoute une variante.
 */
void ajouter_variante_cache(const char *id, SDL_Texture *nouvelle_tex, int angle) {
    if (!gs)
        goto gsvide;
    if (!id || !nouvelle_tex)
        return;

    /* Copie locale pour recherche */
    char lien_recherche[TAILLE_LIEN_GT];
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

/*
 * Récupère une variante (angle).
 */
SDL_Texture *recuperer_texture_variante(const char *lien, int angle) {
    if (!gs)
        goto gsvide;
    if (!lien)
        return NULL;

    char lien_recherche[TAILLE_LIEN_GT];
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