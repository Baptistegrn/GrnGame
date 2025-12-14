#include "../../main.h"
#include "../../logging/logging.h"
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

void init_gestionnaire_textures(GestionnaireTextures* gt, SDL_Renderer* rendu)
{
    if (!gt || !rendu) {
        log_fmt(NiveauLogErreur, "Init gestionnaire textures impossible");
        return;
    }

    gt->capacite = 50;
    gt->taille = 0;
    gt->rendu = rendu;
    gt->entrees = calloc(gt->capacite, sizeof(TextureEntry));
    
    if (!gt->entrees) {
        log_fmt(NiveauLogErreur, "Echec malloc gestionnaire");
        gt->capacite = 0;
    }
}

void agrandir_si_plein(GestionnaireTextures* gt)
{
    if (!gt) return;

    if (gt->taille >= gt->capacite)
    {
        int nouvelle_capacite = gt->capacite + 50;
        TextureEntry* nouvelles_entrees = realloc(gt->entrees, sizeof(TextureEntry) * nouvelle_capacite);

        if (!nouvelles_entrees) {
            log_fmt(NiveauLogErreur, "Echec realloc gestionnaire");
            return;
        }

        gt->entrees = nouvelles_entrees;
        memset(gt->entrees + gt->capacite, 0, sizeof(TextureEntry) * 50);
        gt->capacite = nouvelle_capacite;
    }
}

SDL_Texture* charger_une_texture(GestionnaireTextures* gt, const char* lien_complet)
{
    if (!gt || !lien_complet) return NULL;

    SDL_Texture* existant = recuperer_texture_par_lien(gt, (char*)lien_complet);
    if (existant) return existant;

    SDL_Surface* surface = IMG_Load(lien_complet);
    if (!surface) {
        log_fmt(NiveauLogErreur, "Img introuvable '%s': %s", lien_complet, IMG_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(gt->rendu, surface);
    SDL_FreeSurface(surface);
    if (!tex) return NULL;

    agrandir_si_plein(gt);
    if (gt->taille >= gt->capacite) {
        SDL_DestroyTexture(tex);
        return NULL;
    }

    int index = gt->taille++;
    TextureEntry* entree = &gt->entrees[index];
    
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT - 1);
    entree->id[TAILLE_LIEN_GT - 1] = '\0';
    entree->textures = creer_liste(tex, 0);

    return tex;
}

void charger_toutes_les_textures(GestionnaireTextures* gt, const char* dossier)
{
    if (!gt || !dossier) return;
    char** liste_textures = NULL;
    int nb = 0;

    if (collect_pngs(dossier, &liste_textures, &nb) != 0) return;

    for (int i = 0; i < nb; i++) {
        SDL_Texture* tex = charger_une_texture(gt, liste_textures[i]);
        free(liste_textures[i]);
    }
    free(liste_textures);
}

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures* gt, char* lien)
{
    if (!gt || !lien) return NULL;
    normaliser_chemin(lien); 

    for (int i = 0; i < gt->taille; i++)
    {
        TextureEntry* entree = &gt->entrees[i];
        if (strcmp(entree->id, lien) == 0)
        {
            return recuperer_texture_angle_indice(entree->textures, 0);
        }
    }
    return NULL;
}

void ajouter_variante_cache(GestionnaireTextures* gt, char* id, SDL_Texture* nouvelle_tex, int angle)
{
    if (!gt || !id || !nouvelle_tex) return;

    for (int i = 0; i < gt->taille; i++)
    {
        TextureEntry* entree = &gt->entrees[i];
        if (strcmp(entree->id, id) == 0)
        {
            if (recuperer_texture_angle_indice(entree->textures, angle) == NULL) {
                entree->textures = ajouter_en_tete(entree->textures, nouvelle_tex, angle);
            }
            return;
        }
    }
}

SDL_Texture* recuperer_texture_variante(GestionnaireTextures* gt, char* id, int angle)
{
    if (!gt || !id) return NULL;
    normaliser_chemin(id);
    for (int i = 0; i < gt->taille; i++)
    {
        TextureEntry* entree = &gt->entrees[i];
        if (strcmp(entree->id, id) == 0)
        {
            return recuperer_texture_angle_indice(entree->textures, angle);
        }
    }
    return NULL;
}