#include "../../main.h"

int souris_x() { return gs->entrees->souris_x; }

int souris_y() { return gs->entrees->souris_y; }

int souris_gauche_juste_presse() { return gs->entrees->souris_juste_presse; }

int souris_droite_juste_presse() { return gs->entrees->souris_droite_juste_presse; }

int souris_gauche_presse() { return gs->entrees->souris_presse; }

int souris_droite_presse() { return gs->entrees->souris_droite_presse; }

float dt() { return gs->timing->dt; }

float nb_images() { return gs->timing->fps; }

Uint32 compteur_frame() { return gs->timing->compteur_frames; }

int decalage_x() { return gs->fenetre->decalage_x; }

int decalage_y() { return gs->fenetre->decalage_y; }

int coeff() { return gs->fenetre->coeff; }