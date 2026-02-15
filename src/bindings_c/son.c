/*
 * Bindings C pour le systeme audio.
 */

#include "../moteur/son/son.h"

/* Joue un son avec options de boucle, canal et volume */
void playSound(const char *path, int loop, int channel, int volume) {
    jouer_son(path, loop, channel, volume);
}

/* Arrete un son identifie par son chemin */
void stopSound(const char *path) { arreter_son(path); }

/* Met en pause un son identifie par son chemin */
void pauseSound(const char *path) { pause_son(path); }

/* Reprend un son en pause */
void resumeSound(const char *path) { reprendre_son(path); }

/* Arrete la lecture d'un canal audio */
void stopChannel(int channel) { arreter_canal(channel); }

/* Met en pause un canal audio */
void pauseChannel(int channel) { pause_canal(channel); }

/* Reprend un canal audio en pause */
void resumeChannel(int channel) { reprendre_canal(channel); }

/* Charge tous les sons d'un dossier en memoire */
void loadSongFolder(const char *folder) { charger_tout_les_sons(folder); }

/* Libere tous les sons en memoire */
void freeSongFolder(void) { liberer_gestionnaire_son(); }

/* mettre la cle pour les sons */
void setSongKey(int index, int value) { mettre_son_cle(index, value); }

/* mettre liv pour les sons */
void setSongIv(int index, int value) { mettre_son_iv(index, value); }