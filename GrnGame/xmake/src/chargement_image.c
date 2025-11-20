#include "main.h"
#include "logging.h" // Ajout pour log_fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>

void init_gestionnaire_textures(GestionnaireTextures* gt, SDL_Renderer* rendu)
{
    if (!gt || !rendu)
    {
        log_fmt(NiveauLogErreur, "Initialisation du gestionnaire de textures impossible (paramètre NULL)");
        return;
    }

    gt->capacite = 50;
    gt->taille = 0;
    gt->rendu = rendu;

    gt->entrees = malloc(sizeof(TextureEntry) * gt->capacite);
    if (!gt->entrees)
    {
        log_fmt(NiveauLogErreur, "Échec d'allocation mémoire pour le gestionnaire de textures (capacité: %d)",
                gt->capacite);
        gt->capacite = 0;
    }
}
void agrandir_si_plein(GestionnaireTextures* gt)
{
    if (!gt)
    {
        log_fmt(NiveauLogErreur, "Gestionnaire de textures NULL lors de la réallocation");
        return;
    }

    if (gt->taille >= gt->capacite)
    {
        int nouvelle_capacite = gt->capacite + 50;
        TextureEntry* nouvelles_entrees = realloc(gt->entrees, sizeof(TextureEntry) * nouvelle_capacite);

        if (!nouvelles_entrees)
        {
            log_fmt(NiveauLogErreur, "Échec de réallocation mémoire (capacité: %d -> %d)", gt->capacite,
                    nouvelle_capacite);
            return;
        }

        gt->entrees = nouvelles_entrees;
        gt->capacite = nouvelle_capacite;
    }
}

SDL_Texture* charger_une_texture(GestionnaireTextures* gt, const char* lien_complet)
{
    if (!gt || !lien_complet)
    {
        log_fmt(NiveauLogErreur, "Chargement de texture impossible (paramètre NULL)");
        return NULL;
    }

    SDL_Surface* surface = IMG_Load(lien_complet);
    if (!surface)
    {
        log_fmt(NiveauLogErreur, "Impossible de charger l'image '%s': %s", lien_complet, IMG_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(gt->rendu, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        log_fmt(NiveauLogErreur, "Impossible de créer la texture depuis '%s': %s", lien_complet, SDL_GetError());
        return NULL;
    }

    agrandir_si_plein(gt);

    if (gt->taille >= gt->capacite)
    {
        log_fmt(NiveauLogErreur, "Capacité du gestionnaire de textures dépassée");
        SDL_DestroyTexture(tex);
        return NULL;
    }

    int index = gt->taille++;
    TextureEntry* entree = &gt->entrees[index];
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT - 1);
    entree->id[TAILLE_LIEN_GT - 1] = '\0';
    entree->texture = tex;

    return tex;
}

void charger_toutes_les_textures(GestionnaireTextures* gt, const char* dossier)
{
    if (!gt || !dossier)
    {
        log_fmt(NiveauLogErreur, "Chargement des textures impossible (paramètre NULL)");
        return;
    }
    char** liste_textures = NULL;
    int nb = 0;

    if (collect_pngs(dossier, &liste_textures, &nb) != 0)
    {
        log_fmt(NiveauLogErreur, "Impossible de lister les fichiers PNG dans '%s'", dossier);
        return;
    }

    int nb_echecs = 0;
    for (int i = 0; i < nb; i++)
    {
        SDL_Texture* tex = charger_une_texture(gt, liste_textures[i]);
        if (!tex)
        {
            nb_echecs++;
        }
        free(liste_textures[i]);
    }

    free(liste_textures);

    if (nb_echecs > 0)
    {
        log_fmt(NiveauLogAvertissement, "Attention: %d texture(s) n'ont pas pu être chargée(s) sur %d", nb_echecs, nb);
    }
}

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures* gt, char* lien)
{
    if (!gt || !lien)
    {
        log_fmt(NiveauLogErreur, "Récupération de texture impossible (paramètre NULL)");
        return NULL;
    }


    normaliser_chemin(lien);
    for (int i = 0; i < gt->taille; i++)
    {
        TextureEntry* entree = &gt->entrees[i];
        if (strcmp(entree->id, lien) == 0)
        {
            return entree->texture;
        }
    }
    log_fmt(NiveauLogErreur, "Texture introuvable '%s'", lien);
    return NULL;
}