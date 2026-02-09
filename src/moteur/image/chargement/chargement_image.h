/*
 * Chargement et gestion des ressources images (PNG).
 */

#ifndef IMAGE_H
#define IMAGE_H

#include "../../../proprietes.h"
#include "../liste/liste_chaine.h"
#include <SDL_image.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char id[TAILLE_LIEN]; /* lien */
    Liste *textures;      /* Liste des variantes de textures */
} TextureEntry;

typedef struct GestionnaireTextures {
    TextureEntry *entrees; /* tableau des textures */
    int capacite;          /* capacite du tableau */
    int taille;            /* nombre de textures chargees */
} GestionnaireTextures;

/* Initialise le gestionnaire de textures */
void init_gestionnaire_textures(void);

/* Libere le gestionnaire de textures */
void liberer_gestionnaire_textures(void);

/* Libere le gestionnaire d'images */
void liberer_gestionnaire_image(void);

/* Charge toutes les textures d'un dossier */
void charger_toutes_les_textures(const char *dossier);

/* Recupere une texture par son chemin */
SDL_Texture *recuperer_texture_par_lien(const char *lien);

/* Recupere une texture pivotee du cache */
SDL_Texture *recuperer_texture_variante(const char *id, int angle);

/* Ajoute une variante pivotee au cache */
void ajouter_variante_cache(const char *id, SDL_Texture *nouvelle_tex, int angle);

#ifdef __cplusplus
}
#endif

#endif