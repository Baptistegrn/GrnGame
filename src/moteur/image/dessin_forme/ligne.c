/*
 * Dessin de lignes et de points.
 */

#include "../../../allouer/allouer.h"
#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "dessin.h"
#include <stdbool.h>

/* Dessine une ligne entre deux points avec l'algorithme de Bresenham (pixel art) */
void dessiner_ligne_pixel(float fx0, float fy0, float fx1, float fy1, Uint8 r, Uint8 g, Uint8 b,
                          Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;

    float dx = fabsf(fx1 - fx0);
    float dy = fabsf(fy1 - fy0);
    int nb_pixels = (int)((dx > dy ? dx : dy) + 1);

    unsigned char coeff = gs->fenetre->coeff;
    int dec_x = gs->fenetre->decalage_x;
    int dec_y = gs->fenetre->decalage_y;

    SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);
    SDL_Rect *pixels = malloc_gestion_echec_compteur(sizeof(SDL_Rect) * nb_pixels);
    if (!pixels)
        return;

    float x = fx0;
    float y = fy0;
    float sx = (fx0 < fx1) ? 1.0f : -1.0f;
    float sy = (fy0 < fy1) ? 1.0f : -1.0f;
    float err = (dx > dy ? dx : -dy) / 2.0f;
    float e2;

    for (int i = 0; i < nb_pixels; i++) {
        int ecran_x = ((int)SDL_roundf(x * (float)coeff)) + dec_x;
        int ecran_y = ((int)SDL_roundf(y * (float)coeff)) + dec_y;
        pixels[i] = (SDL_Rect){ecran_x, ecran_y, (int)coeff, (int)coeff};
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x += sx;
        }
        if (e2 < dy) {
            err += dx;
            y += sy;
        }
    }
    SDL_RenderFillRects(gs->fenetre->rendu, pixels, nb_pixels);
    free_gestion_echec_compteur(pixels);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw pixel line");
}

/* dessine une ligne horizontale en float */
void tracer_ligne_horizontale_float(float x, float y, float longueur, Uint8 r, Uint8 g, Uint8 b,
                                    Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;

    unsigned char coeff = gs->fenetre->coeff;

    SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);
    float x_ecran = x * (float)coeff + (float)gs->fenetre->decalage_x;
    float y_ecran = y * (float)coeff + (float)gs->fenetre->decalage_y;

    SDL_Rect ligne = {(int)SDL_roundf(x_ecran), (int)SDL_roundf(y_ecran),
                      (int)SDL_lroundf(longueur * (float)coeff), (int)coeff};
    SDL_RenderFillRect(gs->fenetre->rendu, &ligne);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw horizontal line float");
}

/* dessine une ligne verticale en float */
void tracer_ligne_verticale_float(float x, float y, float longueur, Uint8 r, Uint8 g, Uint8 b,
                                  Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;

    unsigned char coeff = gs->fenetre->coeff;
    SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);
    float ecran_x = x * (float)coeff + (float)gs->fenetre->decalage_x;
    float ecran_y = y * (float)coeff + (float)gs->fenetre->decalage_y;

    SDL_Rect ligne = {(int)SDL_roundf(ecran_x), (int)SDL_roundf(ecran_y), (int)coeff,
                      (int)SDL_lroundf(longueur * (float)coeff)};
    SDL_RenderFillRect(gs->fenetre->rendu, &ligne);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw vertical line float");
}

/* Dessine deux points aux positions (x1,y1) et (x2,y2) de longueur n (float) */
void dessiner_points_n(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                       float n) {
    if (UNLIKELY(!gs))
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;

    SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);
    SDL_Rect pixels[2];

    float screen_x1 = x1 * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y1 = y1 * (float)coeff + (float)gs->fenetre->decalage_y;
    float screen_x2 = x2 * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y2 = y2 * (float)coeff + (float)gs->fenetre->decalage_y;

    Sint16 x1_int = SDL_lroundf(screen_x1);
    Sint16 y1_int = SDL_lroundf(screen_y1);
    Sint16 x2_int = SDL_lroundf(screen_x2);
    Sint16 y2_int = SDL_lroundf(screen_y2);

    Sint16 width = (Sint16)SDL_lroundf(n * (float)coeff);
    pixels[0] = (SDL_Rect){(int)x1_int, (int)y1_int, (int)width, (int)coeff};
    pixels[1] = (SDL_Rect){(int)(x2_int - width), (int)y2_int, (int)width, (int)coeff};

    SDL_RenderFillRects(gs->fenetre->rendu, pixels, 2);

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw points n");
}

/* Dessine deux points uniques aux positions (x1,y1) et (x2,y2) */
void dessiner_points(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;
    SDL_SetRenderDrawBlendMode(gs->fenetre->rendu, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);
    SDL_Rect pixels[2];

    float screen_x1 = x1 * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y1 = y1 * (float)coeff + (float)gs->fenetre->decalage_y;
    float screen_x2 = x2 * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y2 = y2 * (float)coeff + (float)gs->fenetre->decalage_y;

    Sint16 x1_int = SDL_lroundf(screen_x1);
    Sint16 y1_int = SDL_lroundf(screen_y1);
    Sint16 x2_int = SDL_lroundf(screen_x2);
    Sint16 y2_int = SDL_lroundf(screen_y2);

    pixels[0] = (SDL_Rect){(int)x1_int, (int)y1_int, (int)coeff, (int)coeff};
    pixels[1] = (SDL_Rect){(int)x2_int, (int)y2_int, (int)coeff, (int)coeff};

    SDL_RenderFillRects(gs->fenetre->rendu, pixels, 2);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw points");
}