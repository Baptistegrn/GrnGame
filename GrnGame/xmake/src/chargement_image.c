#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>

void init_gestionnaire_textures(GestionnaireTextures* gt, SDL_Renderer* rendu)
{
    if (!gt || !rendu)
    {
        fprintf(stderr, "Erreur: Initialisation du gestionnaire de textures impossible (paramètre NULL)\n");
        return;
    }

    gt->capacite = 50;
    gt->taille = 0;
    gt->rendu = rendu;

    gt->entrees = malloc(sizeof(TextureEntry) * gt->capacite);
    if (!gt->entrees)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour le gestionnaire de textures (capacité: %d)\n",
                gt->capacite);
        gt->capacite = 0;
    }
}
void agrandir_si_plein(GestionnaireTextures* gt)
{
    if (!gt)
    {
        fprintf(stderr, "Erreur: Gestionnaire de textures NULL lors de la réallocation\n");
        return;
    }

    if (gt->taille >= gt->capacite)
    {
        int nouvelle_capacite = gt->capacite + 50;
        TextureEntry* nouvelles_entrees = realloc(gt->entrees, sizeof(TextureEntry) * nouvelle_capacite);

        if (!nouvelles_entrees)
        {
            fprintf(stderr, "Erreur: Échec de réallocation mémoire (capacité: %d -> %d)\n", gt->capacite,
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
        fprintf(stderr, "Erreur: Chargement de texture impossible (paramètre NULL)\n");
        return NULL;
    }

    SDL_Surface* surface = IMG_Load(lien_complet);
    if (!surface)
    {
        fprintf(stderr, "Erreur: Impossible de charger l'image '%s': %s\n", lien_complet, IMG_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(gt->rendu, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        fprintf(stderr, "Erreur: Impossible de créer la texture depuis '%s': %s\n", lien_complet, SDL_GetError());
        return NULL;
    }

    agrandir_si_plein(gt);

    if (gt->taille >= gt->capacite)
    {
        fprintf(stderr, "Erreur: Capacité du gestionnaire de textures dépassée\n");
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
        fprintf(stderr, "Erreur: Chargement des textures impossible (paramètre NULL)\n");
        return;
    }
    char** liste_textures = NULL;
    int nb = 0;

    if (collect_pngs(dossier, &liste_textures, &nb) != 0)
    {
        fprintf(stderr, "Erreur: Impossible de lister les fichiers PNG dans '%s'\n", dossier);
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
        fprintf(stderr, "Attention: %d texture(s) n'ont pas pu être chargée(s) sur %d\n", nb_echecs, nb);
    }
}

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures* gt, char* lien)
{
    if (!gt || !lien)
    {
        fprintf(stderr, "Erreur: Récupération de texture impossible (paramètre NULL)\n");
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
    fprintf(stderr, "Erreur: Texture introuvable '%s'\n", lien);
    return NULL;
}