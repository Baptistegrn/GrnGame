/* gere les modules jeux et le gestionnaire de modules de jeux */

#ifndef MODULE_JEU_H
#define MODULE_JEU_H

#include "camera/camera.h"
#include "hitbox/hitbox.h"
#include <SDL_mutex.h>
#include <SDL_thread.h>
#include <stdbool.h>

/* Structure pour transmettre les donnees de calcul au thread */
typedef struct {
    EntitePlatformes *entites;
    Blocs *blocs;
    Uint32 masque_ignorer;
    float dt;
} DonneesHitbox;

typedef struct GestionnaireModuleJeu {
    Camera *camera; /* camera de la session */

    /* Variables pour la gestion du thread */
    SDL_Thread *hitbox_t; /* thread pour la hitbox */
    SDL_mutex *mutex;     /* protection des donnees */
    SDL_cond *reveil;     /* signal pour reveiller le thread */
    SDL_cond *termine;    /* signal quand le calcul est fini */

    bool thread_present; /* si le thread est bien creer */
    bool thread_actif;   /* pour garder la boucle du thread en vie */
    bool travail_demande;
    bool travail_termine;

    DonneesHitbox donnees; /* donnees de la frame courante */
} GestionnaireModuleJeu;

/* initialise le module de jeu du gestionnaire */
void initialiser_gestionnaire_module_jeu(void);


/* met le thread principal en pause jusqu'a ce que la hitbox soit calculee */
void attendre_calcul_hitbox(void);

/* libere le gestionnaire de module de jeu */
void liberer_gestionnaire_module_jeu(void);

#endif /* MODULE_JEU_H */