#include "../moteur/fenetre/fenetre.h"
#include "../moteur/input/input.h"

/*
 * Vérifie si une touche vient d'être pressée.
 */
int keyJustPressed(const char *key_name) { return touche_juste_presse(key_name); }

/*
 * Vérifie si une touche est maintenue enfoncée.
 */
int keyPressed(const char *key_name) { return touche_enfoncee(key_name); }

/*
 * Affiche ou masque le curseur de la souris.
 */
void showCursor(bool visible) { afficher_curseur(visible); }
