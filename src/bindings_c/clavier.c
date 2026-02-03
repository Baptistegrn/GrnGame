/*
 * Bindings C pour les entrees clavier et souris.
 */

#include "../moteur/entrees/entrees.h"
#include "../moteur/fenetre/fenetre.h"

/* Verifie si une touche vient d'etre pressee */
int keyJustPressed(const char *key_name) { return touche_juste_presse(key_name); }

/* Verifie si une touche est maintenue enfoncee */
int keyPressed(const char *key_name) { return touche_enfoncee(key_name); }

/* Affiche ou masque le curseur de la souris */
void showCursor(bool visible) { afficher_curseur(visible); }
