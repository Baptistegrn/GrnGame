/*
 * Accesseurs pour les constantes globales du moteur.
 * Renvoie les valeurs depuis les structures du gestionnaire.
 */

#include "../../main.h"

/* Renvoie la position X de la souris */
int souris_x() { return gs->entrees->souris_x; }

/* Renvoie la position Y de la souris */
int souris_y() { return gs->entrees->souris_y; }

/* Renvoie 1 si le clic gauche vient juste d'etre presse */
bool souris_gauche_juste_presse() { return gs->entrees->souris_juste_presse; }

/* Renvoie 1 si le clic droit vient juste d'etre presse */
bool souris_droite_juste_presse() { return gs->entrees->souris_droite_juste_presse; }

/* Renvoie le scroll vertical de la molette */
int souris_scroll_vertical() { return (int)gs->entrees->souris_scroll_y; }

/* Renvoie le scroll horizontal de la molette */
int souris_scroll_horizontal() { return (int)gs->entrees->souris_scroll_x; }

/* Renvoie 1 si le clic gauche est maintenu enfonce */
bool souris_gauche_presse() { return gs->entrees->souris_presse; }

/* Renvoie 1 si le clic droit est maintenu enfonce */
bool souris_droite_presse() { return gs->entrees->souris_droite_presse; }

/* Renvoie le delta time entre les frames */
float dt() { return gs->timing->dt; }

/* Renvoie le nombre d'images par seconde cible */
float nb_images() { return gs->timing->fps; }

/* Renvoie le compteur de frames depuis le debut */
Uint32 compteur_frame() { return gs->timing->compteur_frames; }

/* Renvoie le decalage X pour les bandes noires */
int decalage_x() { return gs->fenetre->decalage_x; }

/* Renvoie le decalage Y pour les bandes noires */
int decalage_y() { return gs->fenetre->decalage_y; }

/* Renvoie la largeur actuelle de la fenetre */
int largeur_actuelle() { return gs->fenetre->largeur_actuelle; }

/* Renvoie la hauteur actuelle de la fenetre */
int hauteur_actuelle() { return gs->fenetre->hauteur_actuelle; }

/* Renvoie la largeur de l'univers de jeu */
int largeur_univers() { return gs->fenetre->largeur_univers; }

/* Renvoie la hauteur de l'univers de jeu */
int hauteur_univers() { return gs->fenetre->hauteur_univers; }

/* Renvoie le coefficient de mise a l'echelle */
int coeff() { return gs->fenetre->coeff; }

/* renvoie si la fenetre est minimise */
bool fenetre_est_minimise() { return gs->timing->en_pause; }