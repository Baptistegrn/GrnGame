#include "../moteur/constante/constante.h"

/*
 * Récupère la position X de la souris.
 */
int mouseX() { return souris_x(); }

/*
 * Récupère la position Y de la souris.
 */
int mouseY() { return souris_y(); }

/*
 * Vérifie si le bouton gauche de la souris vient d'être pressé.
 */
int mouseLeftJustPressed() { return souris_gauche_juste_presse(); }

/*
 * Vérifie si le bouton droit de la souris vient d'être pressé.
 */
int mouseRightJustPressed() { return souris_droite_juste_presse(); }

/*
 * Vérifie si le bouton gauche de la souris est maintenu enfoncé.
 */
int mouseLeftPressed() { return souris_gauche_presse(); }

/*
 * Vérifie si le bouton droit de la souris est maintenu enfoncé.
 */
int mouseRightPressed() { return souris_droite_presse(); }

/*
 * Récupère le delta-time (temps écoulé depuis la dernière frame).
 */
float delta() { return dt(); }

/*
 * Récupère les FPS actuels.
 */
float fps() { return nb_images(); }

/*
 * Récupère le nombre de frames écoulées.
 */
unsigned int frameCount() { return compteur_frame(); }

/*
 * Récupère le décalage X de la fenêtre.
 */
int offsetX() { return decalage_x(); }

/*
 * Récupère le décalage Y de la fenêtre.
 */
int offsetY() { return decalage_y(); }
