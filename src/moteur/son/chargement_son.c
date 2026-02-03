/*
 * Chargement des fichiers audio.
 * Gere le chargement des fichiers WAV et la recuperation par lien.
 */

#include "../../main.h"

/* Initialise le gestionnaire de sons */
void init_gestionnaire_son(void) {
    if (!gs)
        goto gsvide;
    GestionnaireSon *sons = gs->sons;

    sons->capacite = 50;
    sons->taille = 0;

    sons->entrees = malloc_gestion_echec_compteur(sizeof(SonEntry) * sons->capacite);
    memset(sons->entrees, 0, sizeof(SonEntry) * sons->capacite);
    return;

gsvide:
    log_message(NiveauLogErreur, "manager is empty during sound manager init function");
}

/* Verifie si le tableau de sons est plein et l'agrandit si necessaire */
void agrandir_si_plein_son(void) {
    if (!gs)
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

/* Charge un fichier son (WAV) en memoire */
Mix_Chunk *charger_un_son(const char *lien_complet) {
    if (!gs)
        goto gsvide;
    if (!lien_complet)
        return NULL;

    GestionnaireSon *sons = gs->sons;

    char lien_norm[TAILLE_LIEN];
    normaliser_chemin_copies(lien_norm, lien_complet);
    Mix_Chunk *existant = recuperer_son_par_lien(lien_norm);
    if (existant)
        return existant;

    Mix_Chunk *son = Mix_LoadWAV(lien_norm);
    if (!son) {
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
    if (!gs)
        goto gsvide;
    if (!lien)
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

/* Scanne un dossier et charge tous les fichiers WAV trouves */
void charger_tous_les_sons(const char *dossier) {
    if (!gs)
        goto gsvide;
    if (!dossier) {
        log_message(NiveauLogErreur, "NULL directory parameter");
        return;
    }

    Fichiers *fichiers = renvoie_fichier_dossier(dossier, "wav", NULL);

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