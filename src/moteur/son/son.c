/*
 * Lecture et controle des sons.
 * Gere les canaux audio, la lecture, la pause et l'arret.
 */

#include "../../main.h"
#include "SDL_stdinc.h"

/* Joue un son sur un canal specifique avec volume et boucles configurables */
void jouer_son(const char *lien, int boucle, int canal, int volume) {
    if (!gs)
        goto gsvide;

    if (!lien) {
        log_message(NiveauLogErreur, "sound path NULL");
        return;
    }

    /* Verification du numero de canal */
    int nb_canaux = TAILLE_CANAL;
    if (canal < -1 || canal >= nb_canaux) {
        log_fmt(NiveauLogErreur, "invalid channel number (%d)", canal);
        return;
    }

    /* Recuperation de la ressource audio */
    Mix_Chunk *son = recuperer_son_par_lien(lien);
    if (!son) {
        log_fmt(NiveauLogErreur, "sound not found '%s'", lien);
        return;
    }

    /* SDL_Mixer: 0 = une fois, -1 = infini, n = n fois */
    int loops = (boucle > 0) ? boucle - 1 : boucle;

    /* Limitation du volume entre 0 et 128 */
    volume = SDL_clamp(volume, 0, 128);

    Mix_VolumeChunk(son, volume);

    /* Lancement de la lecture */
    if (Mix_PlayChannel(canal, son, loops) == -1) {
        log_fmt(NiveauLogErreur, "impossible to play sound '%s': %s", lien, Mix_GetError());
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager empty in play sound function");
}

/* Arrete la lecture d'un son specifique en parcourant tous les canaux */
void arreter_son(const char *lien) {
    if (!gs)
        goto gsvide;

    if (!lien)
        return;

    Mix_Chunk *son = recuperer_son_par_lien(lien);
    if (!son) {
        log_fmt(NiveauLogErreur, "sound not found '%s'", lien);
        return;
    }

    int nb_canaux = TAILLE_CANAL;
    int canaux_arretes = 0;

    /* Arrete tous les canaux jouant ce son */
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_HaltChannel(i);
            canaux_arretes++;
        }
    }

    if (canaux_arretes == 0) {
        log_fmt(NiveauLogAvertissement, "sound '%s' is not playing on any channel", lien);
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager empty in stop sound function");
}

/* Arrete immediatement la lecture sur un canal donne */
void arreter_canal(int canal) {
    int nb_canaux = TAILLE_CANAL;

    if (canal < 0 || canal >= nb_canaux) {
        log_fmt(NiveauLogErreur, "invalid channel number (%d)", canal);
        return;
    }

    if (Mix_Playing(canal))
        Mix_HaltChannel(canal);
}

/* Reprend la lecture d'un canal mis en pause */
void reprendre_canal(int canal) {
    int nb_canaux = TAILLE_CANAL;

    if (canal < 0 || canal >= nb_canaux) {
        log_fmt(NiveauLogErreur, " invalid channel number (%d)", canal);
        return;
    }

    Mix_Resume(canal);
}

/* Met en pause la lecture d'un canal donne */
void pause_canal(int canal) {
    int nb_canaux = TAILLE_CANAL;

    if (canal < 0 || canal >= nb_canaux) {
        log_fmt(NiveauLogErreur, "invalid channel number (%d)", canal);
        return;
    }

    if (Mix_Playing(canal))
        Mix_Pause(canal);
}

/* Met en pause un son specifique en parcourant tous les canaux actifs */
void pause_son(const char *lien) {
    if (!gs)
        goto gsvide;

    if (!lien)
        return;

    Mix_Chunk *son = recuperer_son_par_lien(lien);
    if (!son) {
        log_fmt(NiveauLogErreur, "sound not found '%s'", lien);
        return;
    }

    int nb_canaux = TAILLE_CANAL;
    int canaux_pauses = 0;

    /* Met en pause tous les canaux jouant ce son */
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son && Mix_Playing(i)) {
            Mix_Pause(i);
            canaux_pauses++;
        }
    }

    if (canaux_pauses == 0) {
        log_fmt(NiveauLogAvertissement, "sound '%s' is not playing on any channel", lien);
    }
    return;

gsvide:

    log_message(NiveauLogDebug, "manager empty in pause sound function");
}

/* Reprend la lecture d'un son specifique mis en pause */
void reprendre_son(const char *lien) {
    if (!gs)
        goto gsvide;

    if (!lien)
        return;

    Mix_Chunk *son = recuperer_son_par_lien(lien);
    if (!son) {
        log_fmt(NiveauLogErreur, "error: sound not found '%s'", lien);
        return;
    }

    int nb_canaux = TAILLE_CANAL;

    /* Reprend tous les canaux jouant ce son */
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son)
            Mix_Resume(i);
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager empty in resume sound function");
}