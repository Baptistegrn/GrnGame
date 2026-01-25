#ifndef CARTE_H
#define CARTE_H

#include "../hitbox/hitbox.h"
#include <SDL.h>

/*charge une carte*/
Blocks *charger_carte(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                      int element_vide);

#endif /* CARTE_H */