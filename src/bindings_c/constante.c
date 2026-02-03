/*
 * Bindings C pour les constantes et valeurs du moteur.
 * Expose les informations de souris, temps et fenetre.
 */

#include "../moteur/constante/constante.h"

/* Retourne la position X de la souris en coordonnees jeu */
int mouseX(void) { return souris_x(); }

/* Retourne la position Y de la souris en coordonnees jeu */
int mouseY(void) { return souris_y(); }

/* Verifie si le bouton gauche vient d'etre presse */
int mouseLeftJustPressed(void) { return souris_gauche_juste_presse(); }

/* Verifie si le bouton droit vient d'etre presse */
int mouseRightJustPressed(void) { return souris_droite_juste_presse(); }

/* Verifie si le bouton gauche est maintenu */
int mouseLeftPressed(void) { return souris_gauche_presse(); }

/* Verifie si le bouton droit est maintenu */
int mouseRightPressed(void) { return souris_droite_presse(); }

/* Retourne le delta du scroll vertical (1 ou -1) */
int mouseScrollVertical(void) { return souris_scroll_vertical(); }

/* Retourne le delta du scroll horizontal (1 ou -1) */
int mouseScrollHorizontal(void) { return souris_scroll_horizontal(); }

/* Retourne le delta time en secondes */
float delta(void) { return dt(); }

/* Retourne les FPS actuels */
float fps(void) { return nb_images(); }

/* Retourne le nombre de frames ecoulees */
unsigned int frameCount(void) { return compteur_frame(); }

/* Retourne le decalage X du viewport */
int offsetX(void) { return decalage_x(); }

/* Retourne le decalage Y du viewport */
int offsetY(void) { return decalage_y(); }

/* Retourne la largeur actuelle de la fenetre */
int currentWidth(void) { return largeur_actuelle(); }

/* Retourne la hauteur actuelle de la fenetre */
int currentHeight(void) { return hauteur_actuelle(); }

/* Retourne la largeur de l'univers (resolution de base) */
int universeWidth(void) { return largeur_univers(); }

/* Retourne la hauteur de l'univers (resolution de base) */
int universeHeight(void) { return hauteur_univers(); }