#include "main.h"

#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_gestionnaire_son(GestionnaireSon* gs)
{
    if (!gs)
    {
        fprintf(stderr, "Erreur: Initialisation du gestionnaire de sons impossible (paramètre NULL)\n");
        return;
    }

    gs->capacite = 50;
    gs->taille = 0;
    gs->entrees = malloc(sizeof(SonEntry) * gs->capacite);

    if (!gs->entrees)
    {
        fprintf(stderr, "Erreur: Échec d'allocation mémoire pour le gestionnaire de sons (capacité: %d)\n",
                gs->capacite);
        gs->capacite = 0;
    }
}
void agrandir_si_plein_son(GestionnaireSon* gs)
{
    if (!gs)
    {
        fprintf(stderr, "Erreur: Gestionnaire de sons NULL lors de la réallocation\n");
        return;
    }

    if (gs->taille >= gs->capacite)
    {
        int nouvelle_capacite = gs->capacite + 50;
        SonEntry* nouvelles_entrees = realloc(gs->entrees, sizeof(SonEntry) * nouvelle_capacite);

        if (!nouvelles_entrees)
        {
            fprintf(stderr, "Erreur: Échec de réallocation mémoire (capacité: %d -> %d)\n", gs->capacite,
                    nouvelle_capacite);
            return;
        }

        gs->entrees = nouvelles_entrees;
        gs->capacite = nouvelle_capacite;
    }
}

Mix_Chunk* charger_un_son(GestionnaireSon* gs, const char* lien_complet)
{
    if (!gs || !lien_complet)
    {
        fprintf(stderr, "Erreur: Chargement de son impossible (paramètre NULL)\n");
        return NULL;
    }

    Mix_Chunk* son = Mix_LoadWAV(lien_complet);
    if (!son)
    {
        fprintf(stderr, "Erreur: Impossible de charger le son '%s': %s\n", lien_complet, Mix_GetError());
        return NULL;
    }

    agrandir_si_plein_son(gs);

    if (gs->taille >= gs->capacite)
    {
        fprintf(stderr, "Erreur: Capacité du gestionnaire de sons dépassée\n");
        Mix_FreeChunk(son);
        return NULL;
    }

    int index = gs->taille++;
    SonEntry* entree = &gs->entrees[index];
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT - 1);
    entree->id[TAILLE_LIEN_GT - 1] = '\0';
    entree->son = son;

    return son;
}

Mix_Chunk* recuperer_son_par_lien(GestionnaireSon* gs, const char* lien)
{
    if (!gs || !lien)
    {
        fprintf(stderr, "Erreur: Récupération de son impossible (paramètre NULL)\n");
        return NULL;
    }

    for (int i = 0; i < gs->taille; i++)
    {
        SonEntry* entree = &gs->entrees[i];
        if (strcmp(entree->id, lien) == 0)
        {
            return entree->son;
        }
    }

    fprintf(stderr, "Erreur: Son introuvable '%s'\n", lien);
    return NULL;
}

void charger_tous_les_sons(GestionnaireSon* gs, const char* dossier)
{
    if (!gs || !dossier)
    {
        fprintf(stderr, "Erreur: Chargement des sons impossible (paramètre NULL)\n");
        return;
    }

    char** liste_sons = NULL;
    int nb = 0;

    if (collect_wavs(dossier, &liste_sons, &nb) != 0)
    {
        fprintf(stderr, "Erreur: Impossible de lister les fichiers WAV dans '%s'\n", dossier);
        return;
    }

    int nb_echecs = 0;
    for (int i = 0; i < nb; i++)
    {
        Mix_Chunk* son = charger_un_son(gs, liste_sons[i]);
        if (!son)
        {
            nb_echecs++;
        }
        free(liste_sons[i]);
    }

    free(liste_sons);

    if (nb_echecs > 0)
    {
        fprintf(stderr, "Attention: %d son(s) n'ont pas pu être chargé(s) sur %d\n", nb_echecs, nb);
    }
}
