#include "main.h"
#include "logging.h" // Ajout pour log_fmt

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

void jouer_son(Gestionnaire* gestionnaire,char* lien, int boucle, int canal,int volume)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire ou gestionnaire de sons NULL");
        return;
    }

    if (!lien)
    {
        log_fmt(NiveauLogErreur, "Erreur: Chemin du son NULL");
        return;
    }

    if (canal < -1 || canal >= Mix_AllocateChannels(-1))
    {
        log_fmt(NiveauLogErreur, "Erreur: Numéro de canal invalide (%d)", canal);
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        log_fmt(NiveauLogErreur, "Erreur: Son introuvable '%s'", lien);
        return;
    }

    // Mix_PlayChannel utilise -1 pour jouer une seule fois, 0 pour une boucle infinie
    int loops = (boucle > 0) ? boucle - 1 : boucle;
    if (volume >128)volume =128;
    Mix_VolumeChunk(son, volume);
    if (Mix_PlayChannel(canal, son, loops) == -1)
    {
        log_fmt(NiveauLogErreur, "Erreur: Impossible de jouer le son '%s': %s", lien, Mix_GetError());
    }
}

void arreter_son(Gestionnaire* gestionnaire, char* lien)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire ou gestionnaire de sons NULL");
        return;
    }

    if (!lien)
    {
        log_fmt(NiveauLogErreur, "Erreur: Chemin du son NULL");
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        log_fmt(NiveauLogErreur, "Erreur: Son introuvable '%s'", lien);
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
        log_fmt(NiveauLogAvertissement, "Attention: Le son '%s' n'est joué sur aucun canal", lien);
    }
}
void arreter_canal(int canal)
{
    if (canal < 0)
    {
        log_fmt(NiveauLogErreur, "Erreur: Numéro de canal invalide (%d)", canal);
        return;
    }

    if (canal >= Mix_AllocateChannels(-1))
    {
        log_fmt(NiveauLogErreur, "Erreur: Le canal %d n'existe pas", canal);
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
        log_fmt(NiveauLogErreur, "Erreur: Numéro de canal invalide (%d)", canal);
        return;
    }

    if (canal >= Mix_AllocateChannels(-1))
    {
        log_fmt(NiveauLogErreur, "Erreur: Le canal %d n'existe pas", canal);
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
        log_fmt(NiveauLogErreur, "Erreur: Numéro de canal invalide (%d)", canal);
        return;
    }

    if (canal >= Mix_AllocateChannels(-1))
    {
        log_fmt(NiveauLogErreur, "Erreur: Le canal %d n'existe pas", canal);
        return;
    }

    Mix_Resume(canal);
}

void pause_son(Gestionnaire* gestionnaire,  char* lien)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire ou gestionnaire de sons NULL");
        return;
    }

    if (!lien)
    {
        log_fmt(NiveauLogErreur, "Erreur: Chemin du son NULL");
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        log_fmt(NiveauLogErreur, "Erreur: Son introuvable '%s'", lien);
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
        log_fmt(NiveauLogAvertissement, "Attention: Le son '%s' n'est joué sur aucun canal", lien);
    }
}

void reprendre_son(Gestionnaire* gestionnaire,  char* lien)
{
    if (!gestionnaire || !gestionnaire->sons)
    {
        log_fmt(NiveauLogErreur, "Erreur: Gestionnaire ou gestionnaire de sons NULL");
        return;
    }

    if (!lien)
    {
        log_fmt(NiveauLogErreur, "Erreur: Chemin du son NULL");
        return;
    }

    Mix_Chunk* son = recuperer_son_par_lien(gestionnaire->sons, lien);
    if (!son)
    {
        log_fmt(NiveauLogErreur, "Erreur: Son introuvable '%s'", lien);
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