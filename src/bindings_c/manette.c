/*
 * Bindings C pour les manettes de jeu.
 */

#include "../moteur/entrees/entrees.h"

/* Verifie si un bouton de manette est maintenu enfonce */
int buttonPressed(const char *button_name, unsigned char index) {
    return touche_manette_enfoncee(button_name, index);
}

/* Verifie si un bouton de manette vient d'etre presse */
int buttonJustPressed(const char *button_name, unsigned char index) {
    return touche_manette_juste_presse(button_name, index);
}

/* Initialise une manette a l'index specifie */
void initController(unsigned char index) { init_controller_joysticks(index); }

/* Retourne les valeurs des axes des joysticks avec zone morte */
float *getJoysticks(float dead_zone, unsigned char index) {
    return renvoie_joysticks(dead_zone, index);
}

/* Ferme le controller a l'index specifie */
void closeController(unsigned char index) { fermer_controller(index); }

/* Ferme le joystick a l'index specifie */
void closeJoystick(unsigned char index) { fermer_joystick(index); }

/* Ferme le controller et le joystick a l'index specifie */
void closeTheController(unsigned char index) {
    fermer_controller(index);
    fermer_joystick(index);
}