#include "main.h"

#include <stdio.h>

static UpdateCallback g_update_callback = NULL;

void set_update_callback(UpdateCallback cb)
{
    g_update_callback = cb;
}

void boucle_principale(Gestionnaire* jeu)
{
    if (!jeu)
    {
        fprintf(stderr, "Erreur: Gestionnaire de jeu NULL dans la boucle principale\n");
        return;
    }

    if (!jeu->fond)
    {
        fprintf(stderr, "Erreur: Configuration du fond d'écran manquante\n");
        return;
    }

    if (jeu->fps <= 0)
    {
        fprintf(stderr, "Attention: FPS invalide (%f), correction à 60 FPS\n", jeu->fps);
        jeu->fps = 60;
    }

    Uint32 last_ticks = SDL_GetTicks();
    const float dt_theorique = 1.0f / (float)jeu->fps;

    while (jeu->run)
    {
        Uint32 frame_start = SDL_GetTicks();

        jeu->temps_frame++;
        input_update(jeu, jeu->entrees);

        if (g_update_callback)
        {
            g_update_callback(jeu);
        }
        else
        {
            update(jeu);
        }

        actualiser(jeu, jeu->fond->bande_noir);

        Uint32 frame_time_ms = SDL_GetTicks() - frame_start;
        float frame_time_s = frame_time_ms / 1000.0f;

        if (frame_time_s < dt_theorique)
        {
            SDL_Delay((Uint32)((dt_theorique - frame_time_s) * 1000.0f));
        }

        Uint32 current_ticks = SDL_GetTicks();
        float dt_reel = (current_ticks - last_ticks) / 1000.0f;
        last_ticks = current_ticks;

        jeu->dt = (dt_reel > dt_theorique) ? dt_reel : dt_theorique;
    }

    liberer_jeu(jeu);
}
void update(Gestionnaire* jeu)
{
    if (g_update_callback)
    {
        g_update_callback(jeu);
        return;
    }
    (void)jeu;
}
