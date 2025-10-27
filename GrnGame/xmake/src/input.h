#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <stdbool.h>
#include "main.h"

typedef struct
{
    const char* nom;
    SDL_Scancode code;
} ToucheNom;

typedef struct
{
    const char* nom;
    SDL_GameControllerButton bouton;
} ManetteBoutonNom;

void input_update(Gestionnaire* jeu, GestionnaireEntrees* entrees);

char* normaliser_nom(const char* src);

SDL_Scancode scancode_depuis_nom(const char* nom_non_normalise);
bool touche_juste_presse(Gestionnaire* jeu, const char* touche);
bool touche_enfoncee(Gestionnaire* jeu, const char* touche);

SDL_GameControllerButton bouton_manette_depuis_nom(const char* nom_non_normalise);
bool touche_mannette_juste_presse(Gestionnaire* jeu, const char* touche);
bool touche_mannette_enfoncee(Gestionnaire* jeu, const char* touche);

extern const ToucheNom touches_1[];
extern const ToucheNom touches_2[];
extern const ToucheNom touches_longues[];

extern const ManetteBoutonNom boutons_manette[];

#endif
