/*
 * Bindings C pour les fonctions utilitaires.
 * Initialisation du moteur, logging et controle de la boucle.
 */

#include "../bindings_lua/bindings_lua.h"
#include "../chemin/chemin.h"
#include "../module_jeu/carte/carte.h"
#include "../moteur/boucle/boucle.h"
#include "../moteur/fenetre/fenetre.h"
#include "../moteur/initialisation/initialisation.h"
#include "../moteur/logging/logging.h"

typedef void (*UpdateCallback)(void);
static UpdateCallback g_update_callback = NULL;

/* Fonction trampoline appelee par le moteur a chaque frame */
static void update_trampoline(void) {
    if (g_update_callback) {
        g_update_callback();
    }
}

/* Initialise le moteur et demarre la boucle principale */
void initialize(int height, int width, float fps_target, int black_bars, const char *window_title,
                UpdateCallback update_func) {
    g_update_callback = update_func;
    definir_rappel_mise_a_jour(update_trampoline);
    initialiser(height, width, fps_target, black_bars, window_title);
    boucle_principale();
}

/* Arrete le moteur et ferme l'application */
void stop(void) { arreter_gestionnaire(); }

/* Redimensionne la fenetre et ajuste le mode plein ecran */
void resize(int w, int h, int fullscreen, int fenetre_fullscreen) {
    redimensionner(w, h, fullscreen, fenetre_fullscreen);
}

/* Definit la couleur de fond de l'ecran */
void clearScreen(int red, int green, int blue) { stocker_coloriage(red, green, blue); }

/* Enregistre un message de log (0=Debug, 1=Info, 2=Warning, 3=Error) */
void logMessage(int level, char *message) { log_message((NiveauLog)level, message); }

/* Change le niveau minimum de log affiche */
void setLogLvl(int level) { changer_niveau_log((NiveauLog)level); }

/* Initialise et execute le moteur Lua */
void initializeLua(const char *fichier_chemin_lua) { initialiser_lua(fichier_chemin_lua); }