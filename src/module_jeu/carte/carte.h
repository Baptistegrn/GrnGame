/*
 * Chargement de cartes depuis des fichiers texte.
 */

#ifndef CARTE_H
#define CARTE_H

#include "../hitbox/hitbox.h"
#include <SDL.h>

/*
 * Charge une carte depuis un fichier.
 * Retourne un conteneur de blocs avec leurs positions et types.
 */
Blocs *charger_carte(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                     int element_vide);

#endif /* CARTE_H */