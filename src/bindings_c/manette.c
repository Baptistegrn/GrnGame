#include "../moteur/input/input.h"

/*
 * Vérifie si un bouton de manette est maintenu enfoncé.
 */
int buttonPressed(const char *button_name, unsigned char index) {
    return touche_mannette_enfoncee(button_name, index);
}

/*
 * Vérifie si un bouton de manette vient d'être pressé (front montant).
 */
int buttonJustPressed(const char *button_name, unsigned char index) {
    return touche_mannette_juste_presse(button_name, index);
}

/*
 * Initialise les manettes/joysticks à partir d'un index.
 */
void initController(unsigned char index) { init_controller_joysticks(index); }

/*
 * Récupère les axes des joysticks avec zone morte.
 * Retourne un pointeur vers 6 floats (axes x et y pour 3 joysticks)
 */
float *getJoysticks(float dead_zone, unsigned char index) {
    return renvoie_joysticks(dead_zone, index);
}
/*
 * Ferme la manette.
 */
void closeController(unsigned char index) { fermer_controller(index); }

/*
 * Ferme les joysticks
 */
void closeJoystick(unsigned char index) { fermer_joystick(index); }
/*
 * Ferme la manette et les joysticks.
 */
void closeTheController(unsigned char index) {
    fermer_controller(index);
    fermer_joystick(index);
}