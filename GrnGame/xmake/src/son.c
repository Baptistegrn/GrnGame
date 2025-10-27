

#include "main.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

void jouer_son(Gestionnaire* gestionnaire, const char* lien, int boucle, int canal)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou gestionnaire de sons NULL\n");
        return;
    }

    if (!lien)
    {
        fprintf(stderr, "Erreur: Chemin du son NULL\n");
        return;
    }

    if (canal < -1 || canal >= Mix_AllocateChannels(-1))
    {
        fprintf(stderr, "Erreur: Numéro de canal invalide (%d)\n", canal);
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        fprintf(stderr, "Erreur: Son introuvable '%s'\n", lien);
        return;
    }

    // Mix_PlayChannel utilise -1 pour jouer une seule fois, 0 pour une boucle infinie
    int loops = (boucle > 0) ? boucle - 1 : boucle;

    if (Mix_PlayChannel(canal, son, loops) == -1)
    {
        fprintf(stderr, "Erreur: Impossible de jouer le son '%s': %s\n", lien, Mix_GetError());
    }
}

void arreter_son(Gestionnaire* gestionnaire, const char* lien)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou gestionnaire de sons NULL\n");
        return;
    }

    if (!lien)
    {
        fprintf(stderr, "Erreur: Chemin du son NULL\n");
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        fprintf(stderr, "Erreur: Son introuvable '%s'\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1);
    int canaux_arretes = 0;

    for (int i = 0; i < nb_canaux; i++)
    {
        if (Mix_GetChunk(i) == son)
        {
            Mix_HaltChannel(i);
            canaux_arretes++;
        }
    }

    if (canaux_arretes == 0)
    {
        fprintf(stderr, "Attention: Le son '%s' n'est joué sur aucun canal\n", lien);
    }
}
void arreter_canal(int canal)
{
    if (canal < 0)
    {
        fprintf(stderr, "Erreur: Numéro de canal invalide (%d)\n", canal);
        return;
    }

    if (canal >= Mix_AllocateChannels(-1))
    {
        fprintf(stderr, "Erreur: Le canal %d n'existe pas\n", canal);
        return;
    }

    if (Mix_Playing(canal))
    {
        Mix_HaltChannel(canal);
    }
}

void pause_canal(int canal)
{
    if (canal < 0)
    {
        fprintf(stderr, "Erreur: Numéro de canal invalide (%d)\n", canal);
        return;
    }

    if (canal >= Mix_AllocateChannels(-1))
    {
        fprintf(stderr, "Erreur: Le canal %d n'existe pas\n", canal);
        return;
    }

    if (Mix_Playing(canal))
    {
        Mix_Pause(canal);
    }
}

void reprendre_canal(int canal)
{
    if (canal < 0)
    {
        fprintf(stderr, "Erreur: Numéro de canal invalide (%d)\n", canal);
        return;
    }

    if (canal >= Mix_AllocateChannels(-1))
    {
        fprintf(stderr, "Erreur: Le canal %d n'existe pas\n", canal);
        return;
    }

    Mix_Resume(canal);
}

void pause_son(Gestionnaire* gestionnaire, const char* lien)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou gestionnaire de sons NULL\n");
        return;
    }

    if (!lien)
    {
        fprintf(stderr, "Erreur: Chemin du son NULL\n");
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        fprintf(stderr, "Erreur: Son introuvable '%s'\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1);
    int canaux_pauses = 0;

    for (int i = 0; i < nb_canaux; i++)
    {
        if (Mix_GetChunk(i) == son && Mix_Playing(i))
        {
            Mix_Pause(i);
            canaux_pauses++;
        }
    }

    if (canaux_pauses == 0)
    {
        fprintf(stderr, "Attention: Le son '%s' n'est joué sur aucun canal\n", lien);
    }
}

void reprendre_son(Gestionnaire* gestionnaire, const char* lien)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou gestionnaire de sons NULL\n");
        return;
    }

    if (!lien)
    {
        fprintf(stderr, "Erreur: Chemin du son NULL\n");
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        fprintf(stderr, "Erreur: Son introuvable '%s'\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1);

    for (int i = 0; i < nb_canaux; i++)
    {
        if (Mix_GetChunk(i) == son)
        {
            Mix_Resume(i);
        }
    }
}
