#include "../../main.h"

int souris_x() { return gs->entrees->souris_x; }

int souris_y() { return gs->entrees->souris_y; }

int souris_gauche_juste_presse() { return gs->entrees->souris_juste_presse; }

int souris_droite_juste_presse() { return gs->entrees->souris_droite_juste_presse; }

int souris_scroll_vertical() { return (int)gs->entrees->souris_scroll_y; }

int souris_scroll_horizontal() { return (int)gs->entrees->souris_scroll_x; }

int souris_gauche_presse() { return gs->entrees->souris_presse; }

int souris_droite_presse() { return gs->entrees->souris_droite_presse; }

float dt() { return gs->timing->dt; }

float nb_images() { return gs->timing->fps; }

Uint32 compteur_frame() { return gs->timing->compteur_frames; }

int decalage_x() { return gs->fenetre->decalage_x; }

int decalage_y() { return gs->fenetre->decalage_y; }

int largeur_actuelle() { return gs->fenetre->largeur_actuelle; }
int hauteur_actuelle() { return gs->fenetre->hauteur_actuelle; }

int largeur_univers() { return gs->fenetre->largeur_univers; }
int hauteur_univers() { return gs->fenetre->hauteur_univers; }

int coeff() { return gs->fenetre->coeff; }