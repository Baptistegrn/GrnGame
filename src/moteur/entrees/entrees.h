/*
 * Gestion des entrees (clavier, souris, manette).
 * Structures et API pour la lecture/normalisation des events SDL.
 */

#ifndef ENTREES_H
#define ENTREES_H

#include "../../proprietes.h"
#include <SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Position d'un stick analogique */
typedef struct {
    int x; /* axe horizontal */
    int y; /* axe vertical */
} StickPos;

/* Etats des deux sticks d'une manette (g = gauche, d = droite) */
typedef struct {
    StickPos g; /* stick gauche */
    StickPos d; /* stick droit */
} ManetteEtatJoy;

/* Valeurs brutes des gachettes/trigger (axe) */
typedef struct {
    int trigger_g; /* trigger gauche */
    int trigger_d; /* trigger droit */
} Trigger;

/* Pointeurs SDL pour controller et joystick (si presents) */
typedef struct {
    SDL_GameController *manette; /* interface haut niveau SDL */
    SDL_Joystick *joystick;      /* interface brute SDL_Joystick */
} Manette;

/* Etat global des entrees (mise a jour chaque frame) */
typedef struct GestionnaireEntrees {
    /* Souris */
    int souris_x, souris_y;          /* position en coord. monde (apres coeff/decalage) */
    bool souris_presse;              /* maintien bouton gauche */
    bool souris_juste_presse;        /* front montant bouton gauche */
    bool souris_droite_presse;       /* maintien bouton droit */
    bool souris_droite_juste_presse; /* front montant bouton droit */
    char souris_scroll_y;            /* molette vertical (-1,0,1) */
    char souris_scroll_x;            /* molette horizontal (-1,0,1) */
    char fichier_drop[TAILLE_LIEN];  /* chemin du fichier drop (SDL_DROPFILE) */

    /* Texte saisi via SDL_TEXTINPUT */
    char texte_ecrit[TAILLE_TEXTE_SAISIE];
    int taille_texte_ecrit;       /* longueur courante de `texte_ecrit` */
    char texte_drop[TAILLE_LIEN]; /* texte accompagne un drop (si utilise) */

    /* Clavier - tableaux indexes par SDL_Scancode */
    bool entrees[SDL_NUM_SCANCODES];        /* etat maintien (true si enfoncee) */
    bool entrees_presse[SDL_NUM_SCANCODES]; /* front montant (juste presse) */

    /* Manette - [bouton][index_manette] */
    bool manette[SDL_CONTROLLER_BUTTON_MAX][NB_MANETTES_MAX];        /* maintien */
    bool manette_presse[SDL_CONTROLLER_BUTTON_MAX][NB_MANETTES_MAX]; /* front montant */
    ManetteEtatJoy Joy[NB_MANETTES_MAX];                             /* sticks bruts */
    Trigger trigger[NB_MANETTES_MAX];                                /* triggers bruts */

    /* Instances SDL pour chaque manette connectee */
    Manette man[NB_MANETTES_MAX];
} GestionnaireEntrees;

/* Mapping nom lisible -> scancode */
typedef struct {
    const char *nom;   /* nom lisible (ex: "A", "left") */
    SDL_Scancode code; /* scancode SDL correspondant */
} ToucheNom;

/* Mapping nom lisible -> bouton manette */
typedef struct {
    const char *nom;                 /* nom lisible (ex: "A", "start") */
    SDL_GameControllerButton bouton; /* bouton SDL correspondant */
} ManetteBoutonNom;

/* Tables de correspondance (nom -> code/bouton) */
extern const ToucheNom touches_1[];
extern const ToucheNom touches_2[];
extern const ToucheNom touches_3[];
extern const ToucheNom touches_longues[];
extern const ManetteBoutonNom boutons_manette[];

/* Mise a jour de tous les etats d'entree a chaque frame (SDL_PollEvent) */
void mise_a_jour_input(void);

/*--- Clavier ---*/
/* Retourne true si `touche` a ete presse durant la frame courante (front montant). */
bool touche_juste_presse(const char *touche);
/* Retourne true si `touche` est actuellement enfoncee (maintien). */
bool touche_enfoncee(const char *touche);

/*--- Manette ---*/
/* Initialise les structures et ouvre la manette a `index`. Renvoie true si OK. */
bool init_controller_joysticks(unsigned char index);
/* Ferme et libere la manette a `index` (controller SDL). */
void fermer_controller(unsigned char index);
/* Ferme et libere le joystick brut a `index` (SDL_Joystick). */
void fermer_joystick(unsigned char index);

/*--- Texte / clipboard ---*/
/* Copie `source` (SDL_TEXTINPUT) dans le buffer interne `texte_ecrit`. */
void copie_texte_saisie(char *source);
/* Accesseur en lecture seule au texte saisi. */
const char *lire_texte_saisie(void);

/*--- Requete manette ---*/
/*
 * Retourne un tableau `float[6]` alloue dynamiquement (caller doit free()):
 * [0]=left_x, [1]=left_y, [2]=right_x, [3]=right_y, [4]=trigger_left, [5]=trigger_right.
 * Les valeurs sont normalisees entre -1.0 et 1.0 et la dead_zone est appliquee.
 */
float *renvoie_joysticks(float dead_zone, unsigned char index);
/* Retourne true si le bouton `touche` de la manette `index` vient d'etre presse. */
bool touche_manette_juste_presse(const char *touche, unsigned char index);
/* Retourne true si le bouton `touche` de la manette `index` est maintenu. */
bool touche_manette_enfoncee(const char *touche, unsigned char index);

/*--- Conversion / utilitaires ---*/
/* Normalise un nom (retourne chaine allouee, caller doit free()). */
char *normaliser_nom(const char *src);
/* Convertit un nom lisible en SDL_Scancode (SDL_SCANCODE_UNKNOWN si inconnu). */
SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise);
/* Convertit un nom lisible en SDL_GameControllerButton (INVALID si inconnu). */
SDL_GameControllerButton bouton_manette_depuis_nom(const char *nom_non_normalise);

/*--- Liberation ---*/
/* Libere les structures internes relatives aux entrees. */
void liberer_entrees(void);
/* Ferme/clean les objets manette (controllers/joysticks). */
void liberer_manette(void);

#ifdef __cplusplus
}
#endif

#endif /* ENTREES_H */