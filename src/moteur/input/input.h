/*
 * Gestion des entrées (clavier, souris, manette).
 */

#ifndef INPUT_H
#define INPUT_H

#include "../../proprietes.h"
#include <SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x;
    int y;
} StickPos;

typedef struct {
    StickPos g;
    StickPos d;
} ManetteEtatJoy;

typedef struct {
    int trigger_g;
    int trigger_d;
} Trigger;

typedef struct {
    SDL_GameController *manette;
    SDL_Joystick *joystick;
} Manette;

typedef struct GestionnaireEntrees {
    int souris_x, souris_y;
    bool souris_presse;
    bool souris_juste_presse;
    bool souris_droite_presse;
    bool souris_droite_juste_presse;
    char souris_scroll_y;
    char souris_scroll_x;

    bool entrees[SDL_NUM_SCANCODES];
    bool entrees_presse[SDL_NUM_SCANCODES];

    bool manette[SDL_CONTROLLER_BUTTON_MAX][NB_MANETTES_MAX];
    bool manette_presse[SDL_CONTROLLER_BUTTON_MAX][NB_MANETTES_MAX];
    ManetteEtatJoy Joy[NB_MANETTES_MAX];
    Trigger trigger[NB_MANETTES_MAX];

    Manette man[NB_MANETTES_MAX];
} GestionnaireEntrees;

typedef struct {
    const char *nom;
    SDL_Scancode code;
} ToucheNom;

typedef struct {
    const char *nom;
    SDL_GameControllerButton bouton;
} ManetteBoutonNom;

/* Constantes pour le clavier et la souris */
extern const ToucheNom touches_1[];
extern const ToucheNom touches_2[];
extern const ToucheNom touches_3[];
extern const ToucheNom touches_longues[];
extern const ManetteBoutonNom boutons_manette[];

void mise_a_jour_input(void);

bool touche_juste_presse(const char *touche);
bool touche_enfoncee(const char *touche);

bool init_controller_joysticks(unsigned char index);
void fermer_controller(unsigned char index);
void fermer_joystick(unsigned char index);

bool touche_mannette_juste_presse(const char *touche, unsigned char index);
bool touche_mannette_enfoncee(const char *touche, unsigned char index);
float *renvoie_joysticks(float dead_zone, unsigned char index);

char *normaliser_nom(const char *src);
SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise);
SDL_GameControllerButton bouton_manette_depuis_nom(const char *nom_non_normalise);

#ifdef __cplusplus
}
#endif

#endif