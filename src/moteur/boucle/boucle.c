/*
 * Boucle principale du moteur de jeu.
 * Gere le cycle: Entrees -> Mise a jour -> Rendu -> Timing.
 */

typedef void (*RappelMiseAJour)(void);

#include "../../main.h"

static RappelMiseAJour g_rappel_mise_a_jour = NULL;

/* Definit la fonction de rappel pour les mises a jour de logique */
void definir_rappel_mise_a_jour(RappelMiseAJour cb) { g_rappel_mise_a_jour = cb; }

/* Execute la logique de mise a jour via le callback si defini */
void mettre_a_jour(void) {
    if (g_rappel_mise_a_jour)
        g_rappel_mise_a_jour();
}

/* Arrete proprement la boucle principale */
void arreter_gestionnaire(void) {
    if (gs && gs->timing)
        gs->timing->run = false;
}

/*
 * Boucle principale du jeu.
 * Maintient un framerate constant en utilisant des delais.
 * Calcule le delta time reel entre chaque frame.
 */
void boucle_principale(void) {
    if (!gs)
        goto gsvide;
    /* si pause */
    if (gs->timing->en_pause) {
        log_message(NiveauLogInfo, "main loop ended due to pause");
        return;
    }
    GestionnaireTiming *timing = gs->timing;
    Uint32 frame_debut_avant_while = SDL_GetTicks();
    const float dt_theorique = 1.0f / (float)timing->fps;

    while (timing->run) {
        /* Si en pause, on attend et on traite seulement les evenements */
        if (timing->en_pause) {
            /* mise a jour des events pour restaurer l'etat */
            mise_a_jour_input();
            SDL_Delay(100);
            /* Reset du temps pour eviter un dt enorme a la reprise */
            frame_debut_avant_while = SDL_GetTicks();
            continue;
        }

        Uint32 frame_debut = SDL_GetTicks();
        timing->compteur_frames++;

        /* Traitement des entrees */
        mise_a_jour_input();

        /* Mise a jour de la logique du jeu */
        mettre_a_jour();

        /* Rendu de la frame */
        actualiser();
#ifdef DEBUG_MODE
        actualiser_rechargement();
#endif
        /* Limitation du framerate avec delai si necessaire */
        Uint32 frame_temps_ms = SDL_GetTicks() - frame_debut;
        float frame_temps_s = frame_temps_ms / 1000.0f;

        if (frame_temps_s < dt_theorique) {
            Uint32 delai = (Uint32)((dt_theorique - frame_temps_s) * 1000.0f);
            /* si minimise on augmente le delai */
            if (gs->timing->minimise) {
                delai *= 10;
            }
            SDL_Delay(delai);
        }

        /* Calcul du delta time reel */
        Uint32 frame_actuelle = SDL_GetTicks();
        float dt_reel = (frame_actuelle - frame_debut_avant_while) / 1000.0f;
        frame_debut_avant_while = frame_actuelle;
        timing->dt = (dt_reel > dt_theorique) ? dt_reel : dt_theorique;
    }
    liberer_lua();
    liberer_gs();

    // log_message(NiveauLogDebug, "main loop ended normally");
    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in main loop function");
}