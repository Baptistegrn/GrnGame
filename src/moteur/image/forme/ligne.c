#include "../../../main.h"
#include "SDL_render.h"
#include <stdlib.h>

/*
 * Dessine une ligne entre deux points en utilisant l'algorithme de Bresenham.
 * Convertit les coordonnées du jeu en pixels écran selon le coefficient
 * d'échelle.
 */
void dessiner_ligne_pixel(float x0, float y0, float x1, float y1, Uint8 r, Uint8 g, Uint8 b) {
    if (!gs)
        goto gsvide;
    unsigned int coeff = gs->fenetre->coeff;
    int decalage_x = gs->fenetre->decalage_x;
    int decalage_y = gs->fenetre->decalage_y;

    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, 255);

    float screen_x0 = x0 * (float)coeff + (float)decalage_x;
    float screen_y0 = y0 * (float)coeff + (float)decalage_y;
    float screen_x1 = x1 * (float)coeff + (float)decalage_x;
    float screen_y1 = y1 * (float)coeff + (float)decalage_y;

    Sint16 x0_int = SDL_lroundf(screen_x0);
    Sint16 y0_int = SDL_lroundf(screen_y0);
    Sint16 x1_int = SDL_lroundf(screen_x1);
    Sint16 y1_int = SDL_lroundf(screen_y1);

    Sint16 dx = abs(x1_int - x0_int);
    Sint16 dy = abs(y1_int - y0_int);
    Sint16 sx = x0_int < x1_int ? 1 : -1;
    Sint16 sy = y0_int < y1_int ? 1 : -1;
    Sint16 err = (dx > dy ? dx : -dy) / 2;
    Sint16 e2;

    int n = 10;
    SDL_Rect *pixels = xmalloc(sizeof(SDL_Rect) * n);
    int taille = 0;

    while (true) {
        if (taille >= n) {
            n = n * 2;
            pixels = xrealloc(pixels, sizeof(SDL_Rect) * n);
        }

        pixels[taille] = (SDL_Rect){(int)x0_int, (int)y0_int, (int)coeff, (int)coeff};
        taille++;

        if (x0_int == x1_int && y0_int == y1_int)
            break;

        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0_int += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0_int += sy;
        }
    }

    SDL_RenderFillRects(gs->fenetre->rendu, pixels, taille);
    xfree(pixels);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw pixel line");
}

/*
 * Dessine une ligne horizontale simple remplie de pixels.
 * Applique l'échelle et le décalage écran.
 */
void tracer_ligne_horizontale(float x_start, float y, Sint16 w, Uint8 r, Uint8 g, Uint8 b) {
    if (!gs)
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, 255);

    float screen_x_start = x_start * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y = y * (float)coeff + (float)gs->fenetre->decalage_y;

    Sint16 x_start_int = SDL_lroundf(screen_x_start);
    Sint16 y_int = SDL_lroundf(screen_y);

    SDL_Rect ligne = {(int)x_start_int, (int)y_int, (int)(w * coeff), (int)coeff};
    SDL_RenderFillRect(gs->fenetre->rendu, &ligne);
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw horizontal line");
}

/*
 * Dessine deux points aux positions (x1,y1) et (x2,y2) de longueur n.
 * Utilisé pour tracer les contours de formes comme les cercles.
 */
void dessiner_points_n(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b,
                       Sint16 n) {
    if (!gs)
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, 255);
    SDL_Rect pixels[2];

    float screen_x1 = x1 * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y1 = y1 * (float)coeff + (float)gs->fenetre->decalage_y;
    float screen_x2 = x2 * (float)coeff + (float)gs->fenetre->decalage_x;
    float screen_y2 = y2 * (float)coeff + (float)gs->fenetre->decalage_y;

    Sint16 x1_int = SDL_lroundf(screen_x1);
    Sint16 y1_int = SDL_lroundf(screen_y1);
    Sint16 x2_int = SDL_lroundf(screen_x2);
    Sint16 y2_int = SDL_lroundf(screen_y2);

    pixels[0] = (SDL_Rect){(int)x1_int, (int)y1_int, (int)(coeff * n), (int)coeff};
    pixels[1] = (SDL_Rect){(int)(x2_int - n), (int)y2_int, (int)(coeff * n), (int)coeff};

    SDL_RenderFillRects(gs->fenetre->rendu, pixels, 2);

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw points n");
}

/*
 * Dessine deux points uniques aux positions (x1,y1) et (x2,y2).
 * Chaque point a une largeur de 1 pixel.
 */
void dessiner_points(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b) {
    if (!gs)
        goto gsvide;
    unsigned char coeff = gs->fenetre->coeff;
    SDL_SetRenderDrawColor(gs->fenetre->rendu, r, g, b, 255);
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