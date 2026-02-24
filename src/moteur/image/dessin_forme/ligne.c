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
void dessiner_ligne_pixel(float x0, float y0, float x1, float y1, Uint8 r, Uint8 g, Uint8 b,
                          Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;
    int decalage_x = gs->fenetre->decalage_x;
    int decalage_y = gs->fenetre->decalage_y;

    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, a);

    /* Bresenham */
    float dx = fabsf(x1 - x0);
    float dy = fabsf(y1 - y0);
    float sx = x0 < x1 ? 1.0f : -1.0f;
    float sy = y0 < y1 ? 1.0f : -1.0f;
    float err = (dx > dy ? dx : -dy) / 2.0f;
    float e2;

    int capacite = 50;
    SDL_Rect *pixels = malloc_gestion_echec_compteur(sizeof(SDL_Rect) * capacite);
    int taille = 0;

    while (true) {
        if (taille >= capacite) {
            capacite = capacite * 2;
            pixels = realloc_gestion_echec_compteur(pixels, sizeof(SDL_Rect) * capacite);
        }

        /* Redimensionner */
        int ecran_x = (int)SDL_roundf(x0 * (float)coeff) + decalage_x;
        int ecran_y = (int)SDL_roundf(y0 * (float)coeff) + decalage_y;
        pixels[taille] = (SDL_Rect){ecran_x, ecran_y, (int)coeff, (int)coeff};
        taille++;

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }

    SDL_RenderFillRects(gs->fenetre->rendu, pixels, taille);
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