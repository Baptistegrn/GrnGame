#include <SDL.h>
#include <stdbool.h>

typedef struct Gestionnaire Gestionnaire;

typedef struct {
    int x;
    int y;
} StickPos;

typedef struct {
    StickPos left;
    StickPos right;
} ControllerEtatJoy;

typedef struct {
    int triggerleft;
    int triggerright;
} Trigger;

typedef struct GestionnaireEntrees {
    int mouse_x, mouse_y;
    bool mouse_pressed;
    bool mouse_just_pressed;
    bool mouse_right_pressed;
    bool mouse_right_just_pressed;

    bool keys[SDL_NUM_SCANCODES];
    bool keys_pressed[SDL_NUM_SCANCODES];

    bool controller[SDL_CONTROLLER_BUTTON_MAX];
    bool controller_pressed[SDL_CONTROLLER_BUTTON_MAX];
    ControllerEtatJoy Joy;
    Trigger trigger;
} GestionnaireEntrees;



typedef struct {
  const char *nom;
  SDL_Scancode code;
} ToucheNom;

typedef struct {
  const char *nom;
  SDL_GameControllerButton bouton;
} ManetteBoutonNom;

void input_update(Gestionnaire *jeu, GestionnaireEntrees *entrees);
char *normaliser_nom(const char *src);

SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise);
bool touche_juste_presse(Gestionnaire *jeu, const char *touche);
bool touche_enfoncee(Gestionnaire *jeu, const char *touche);

SDL_GameControllerButton
bouton_manette_depuis_nom(const char *nom_non_normalise);
bool touche_mannette_juste_presse(Gestionnaire *jeu, const char *touche);
bool touche_mannette_enfoncee(Gestionnaire *jeu, const char *touche);

extern const ToucheNom touches_1[];
extern const ToucheNom touches_2[];
extern const ToucheNom touches_3[];
extern const ToucheNom touches_longues[];
extern const ManetteBoutonNom boutons_manette[];
