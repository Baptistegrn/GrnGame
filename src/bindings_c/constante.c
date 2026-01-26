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

/*retourne le scrool vertical de la souris 1 ou -1*/
int mouseScrollVertical() { return souris_scroll_vertical(); }
/*retourne le scrool horizontal de la souris 1 ou -1*/
int mouseScrollHorizontal() { return souris_scroll_horizontal(); }

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

/*
 * Récupère la largeur actuelle de la fenêtre.
 */
int currentWidth() { return largeur_actuelle(); }
/*
 * Récupère la hauteur actuelle de la fenêtre.
 */
int currentHeight() { return hauteur_actuelle(); }
/*
 * Récupère la largeur de l'univers (résolution de base du jeu).
 */
int universeWidth() { return largeur_univers(); }

/*
 * Récupère la hauteur de l'univers (résolution de base du jeu).
 */
int universeHeight() { return hauteur_univers(); }