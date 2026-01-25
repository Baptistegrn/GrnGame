#include "../module_jeu/carte/carte.h"
#include "../moteur/boucle/boucle.h"
#include "../moteur/fenetre/fenetre.h"
#include "../moteur/initialisation/initialisation.h"
#include "../moteur/logging/logging.h"

typedef void (*UpdateCallback)(void);
static UpdateCallback g_update_callback = NULL;

/*
 * Trampoline C → C : fonction appelée par le moteur C,
 * qui appelle le callback de mise à jour.
 */
static void update_trampoline() {
    if (g_update_callback) {
        g_update_callback();
    }
}

/*
 * Initialise le moteur graphique et démarre la boucle principale.
 * Le callback update_func est appelé à chaque frame.
 */
void initialize(int height, int width, float fps_target, int black_bars, const char *window_title,
                const char *log_path, UpdateCallback update_func) {

    g_update_callback = update_func;

    definir_rappel_mise_a_jour(update_trampoline);

    initialiser(height, width, fps_target, black_bars, window_title, log_path);

    boucle_principale();
}

/*
 * Arrête le moteur et ferme l'application.
 */
void stop() { arreter_gestionnaire(); }

/*
 * Redimensionne la fenêtre et ajuste le mode plein écran.
 */
void resize(int w, int h, int fullscreen) { redimensionner(w, h, fullscreen); }

/*
 * Efface l'écran avec la couleur spécifiée.
 */
void clearScreen(int red, int green, int blue) { stocker_coloriage(red, green, blue); }

/*
 * Enregistre un message de log avec le niveau spécifié.
 * level: 0=Debug, 1=Info, 2=Warning, 3=Error
 */
void logMessage(int level, char *message) { log_message((NiveauLog)level, message); }

/*
 * Change le niveau minimum de log affiché.
 */
void setLogLvl(int level) { changer_niveau_log((NiveauLog)level); }