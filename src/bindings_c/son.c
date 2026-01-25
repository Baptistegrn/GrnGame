#include "../moteur/son/son.h"

/*
 * Lance la lecture d'un son avec options de boucle et canal.
 */
void playSound(const char *path, int loop, int channel, int volume) {
    jouer_son(path, loop, channel, volume);
}

/*
 * Arrête la lecture d'un son identifié par son chemin.
 */
void stopSound(const char *path) { arreter_son(path); }

/*
 * Met en pause la lecture d'un son identifié par son chemin.
 */
void pauseSound(const char *path) { pause_son(path); }

/*
 * Reprend la lecture d'un son en pause.
 */
void resumeSound(const char *path) { reprendre_son(path); }

/*
 * Arrête la lecture d'un canal audio spécifique.
 */
void stopChannel(int channel) { arreter_canal(channel); }

/*
 * Met en pause la lecture d'un canal audio spécifique.
 */
void pauseChannel(int channel) { pause_canal(channel); }

/*
 * Reprend la lecture d'un canal audio en pause.
 */
void resumeChannel(int channel) { reprendre_canal(channel); }

/*
 * Charge tous les fichiers sonores d'un dossier en mémoire.
 */
void loadSongFolder(const char *folder) { charger_tous_les_sons(folder); }

/*
 * Libère tous les sons en mémoire.
 */
void freeSongFolder() { liberer_gestionnaire_son(); }
