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
    char id[TAILLE_LIEN_GT];
    Liste *textures;
} TextureEntry;

typedef struct GestionnaireTextures {
    TextureEntry *entrees;
    int capacite;
    int taille;
} GestionnaireTextures;

void init_gestionnaire_textures(void);
void liberer_gestionnaire_textures(void);
void liberer_gestionnaire_image(void);
void charger_toutes_les_textures(const char *dossier);

SDL_Texture *recuperer_texture_par_lien(const char *lien);
SDL_Texture *recuperer_texture_variante(const char *id, int angle);
void ajouter_variante_cache(const char *id, SDL_Texture *nouvelle_tex, int angle);

#ifdef __cplusplus
}
#endif

#endif