/*
 * Bindings C pour les fonctions utilitaires.
 * Initialisation du moteur, logging et controle de la boucle.
 */

#include "../moteur/boucle/boucle.h"
#include "../moteur/fenetre/fenetre.h"
#include "../moteur/initialisation/initialisation.h"
#include "../moteur/logging/logging.h"

typedef void (*UpdateCallback)(void);

/* Initialise le moteur et demarre la boucle principale */
void initialize(int height, int width, float fps_target, bool black_bars, const char *window_title,
                UpdateCallback update_func) {
    definir_rappel_mise_a_jour(update_func);
    initialiser(height, width, fps_target, black_bars, window_title);
    boucle_principale();
}

/* Arrete le moteur et ferme l'application */
void stop(void) { arreter_gestionnaire(); }

/* mode plein ecran */
void fullscreen() { passer_plein_ecran(); }

/* mode fenetre plein ecran */
void windowedFullscreen() { passer_fenetre_maximisee(); }

/* mode fenetre */
void windowed(int width, int height) { passer_fenetre_taille(width, height); }

/* mode fenetre avec coeff et centrage */
void windowedMinimised(int coeff) { passer_fenetre_coeff(coeff); }

/* Definit la couleur de fond de l'ecran */
void clearScreen(int red, int green, int blue) { stocker_coloriage(red, green, blue); }

/* Enregistre un message de log (0=Debug, 1=Info, 2=Warning, 3=Error) */
void logMessage(int level, const char *message) { log_message((NiveauLog)level, message); }

/* Change le niveau minimum de log affiche */
void setLogLvl(int level) { changer_niveau_log((NiveauLog)level); }
