/*
 * Gestion de la boucle principale du moteur.
 */

#ifndef BOUCLE_H
#define BOUCLE_H

#include <SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gestionnaire Gestionnaire;

typedef void (*RappelMiseAJour)(void);

typedef struct GestionnaireTiming {
  bool run;
  float dt;
  float fps;
  Uint32 compteur_frames;
} GestionnaireTiming;

void definir_rappel_mise_a_jour(RappelMiseAJour cb);
void boucle_principale();
void arreter_gestionnaire();

#ifdef __cplusplus
}
#endif

#endif /* BOUCLE_H */
