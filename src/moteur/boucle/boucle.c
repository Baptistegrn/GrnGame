typedef void (*RappelMiseAJour)(void);

#include "../../main.h"

static RappelMiseAJour g_rappel_mise_a_jour = NULL;

/*
 * Définit la fonction de rappel pour les mises à jour de logique.
 */
void definir_rappel_mise_a_jour(RappelMiseAJour cb) { g_rappel_mise_a_jour = cb; }

/*
 * Exécute la logique de mise à jour via le callback si défini.
 */
void mettre_a_jour(void) {
    if (g_rappel_mise_a_jour)
        g_rappel_mise_a_jour();
}

/*
 * Arrête proprement la boucle principale.
 */
void arreter_gestionnaire(void) {
    if (gs && gs->timing)
        gs->timing->run = false;
}

/*
 * Boucle principale du jeu. Cycle: Input → Update → Render → Timing.
 * Maintient un framerate constant en utilisant des délais et calcule le delta
 * time réel.
 */
void boucle_principale(void) {
    if (!gs)
        goto gsvide;

    GestionnaireTiming *timing = gs->timing;
    Uint32 frame_debut_avant_while = SDL_GetTicks();
    const float dt_theorique = 1.0f / (float)timing->fps;

    while (timing->run) {
        Uint32 frame_debut = SDL_GetTicks();
        timing->compteur_frames++;

        /* Traitement des entrées */
        mise_a_jour_input();

        /* Mise à jour de la logique du jeu */
        mettre_a_jour();

        /* Rendu de la frame */
        actualiser();

        /* Limitation du framerate avec délai si nécessaire */
        Uint32 frame_temps_ms = SDL_GetTicks() - frame_debut;
        float frame_temps_s = frame_temps_ms / 1000.0f;

        if (frame_temps_s < dt_theorique) {
            Uint32 delai = (Uint32)((dt_theorique - frame_temps_s) * 1000.0f);
            SDL_Delay(delai);
        }

        /* Calcul du delta time réel */
        Uint32 frame_actuelle = SDL_GetTicks();
        float dt_reel = (frame_actuelle - frame_debut_avant_while) / 1000.0f;
        frame_debut_avant_while = frame_actuelle;
        timing->dt = (dt_reel > dt_theorique) ? dt_reel : dt_theorique;
    }

    liberer_gs();

    log_message(NiveauLogDebug, "main loop ended normally");
    return;

gsvide:
    log_message(NiveauLogDebug, "empty manager in main loop function");
}