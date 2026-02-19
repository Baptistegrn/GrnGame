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
typedef struct fondActualiser fondActualiser;
typedef struct TableauImage TableauImage;
typedef struct Fichiers Fichiers;

typedef void (*RappelMiseAJour)(void);

typedef struct GestionnaireTiming {
    bool run;               /* indique si la boucle tourne */
    float dt;               /* delta time */
    float fps;              /* frames per second */
    Uint32 compteur_frames; /* compteur de frames */
    bool en_pause;          /* indique si le jeu est en pause */
    bool minimise;          /* indique si la fenetre est minimise */
} GestionnaireTiming;

typedef struct GestionnaireFrameActuelle {
    fondActualiser *fond;   /* fond de la frame */
    TableauImage *image;    /* images de la frame */
    Fichiers *fichiers_lua; /* fichiers lua de la frame */
} GestionnaireFrameActuelle;

void definir_rappel_mise_a_jour(RappelMiseAJour cb);
void boucle_principale(void);
void arreter_gestionnaire(void);

/* initialise le gestionnaire de frame */
void initialiser_gestionnaire_frame(void);

/* initialise le gestionnaire de timing */
void initialiser_gestionnaire_timing(void);

/* Fonctions de liberation */
void liberer_timing(void);

#ifdef __cplusplus
}
#endif

#endif /* BOUCLE_H */
