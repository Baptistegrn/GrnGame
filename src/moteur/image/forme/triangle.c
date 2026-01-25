#include "../../../main.h"

/*
 * Dessine un triangle vide en traçant les contours ligne par ligne.
 * La largeur diminue en descendant pour créer l'effet triangulaire.
 * Le taux de diminution dépend du rapport largeur/hauteur.
 */
void dessiner_triangle_vide(float posx, float posy, Sint16 largeur, Sint16 hauteur, Uint8 r,
                            Uint8 g, Uint8 b) {
    if (!gs)
        goto gsvide;

    Sint16 current_w = largeur;
    float current_y = posy + (float)hauteur;
    float y_base = posy + (float)hauteur;

    if (largeur >= hauteur) {
        while (current_w > 0 && current_y >= posy) {
            float x_start = posx + ((float)largeur - (float)current_w) / 2.0f;
            float x_end = x_start + (float)current_w - 1.0f;

            if (current_y == y_base) {
                tracer_ligne_horizontale(x_start, current_y, current_w, r, g, b);
            } else {
                dessiner_points(x_start, current_y, x_end, current_y, r, g, b);
            }

            current_y -= 1.0f;
            Sint16 diff = largeur - hauteur;
            current_w -= (diff > 50) ? 4 : 2;
        }
    } else {
        Sint16 steps = largeur / 2;
        if (steps == 0)
            steps = 1;
        Sint16 epaisseur = hauteur / steps;
        if (epaisseur < 1)
            epaisseur = 1;
        Sint16 count_epaisseur = 0;

        while (current_w > 0 && current_y >= posy) {
            float x_start = posx + ((float)largeur - (float)current_w) / 2.0f;
            float x_end = x_start + (float)current_w - 1.0f;

            if (current_y == y_base) {
                tracer_ligne_horizontale(x_start, current_y, current_w, r, g, b);
            } else {
                dessiner_points(x_start, current_y, x_end, current_y, r, g, b);
            }

            current_y -= 1.0f;
            count_epaisseur++;

            if (count_epaisseur >= epaisseur) {
                current_w -= 2;
                count_epaisseur = 0;
            }
        }
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw empty triangle");
}

/*
 * Dessine un triangle rempli en traçant des lignes horizontales qui diminuent
 * en hauteur. Le taux de diminution est adapté au ratio largeur/hauteur pour un
 * résultat bien proportionné.
 */
void dessiner_triangle_plein(float posx, float posy, Sint16 largeur, Sint16 hauteur, Uint8 r,
                             Uint8 g, Uint8 b) {
    if (!gs)
        goto gsvide;

    Sint16 current_w = largeur;
    float current_y = posy + (float)hauteur;

    if (largeur >= hauteur) {
        while (current_w > 0 && current_y >= posy) {
            float x_start = posx + ((float)largeur - (float)current_w) / 2.0f;
            tracer_ligne_horizontale(x_start, current_y, current_w, r, g, b);

            current_y -= 1.0f;
            Sint16 diff = largeur - hauteur;
            current_w -= (diff > 50) ? 4 : 2;
        }
    } else {
        Sint16 steps = largeur / 2;
        if (steps == 0)
            steps = 1;
        Sint16 epaisseur = hauteur / steps;
        if (epaisseur < 1)
            epaisseur = 1;
        Sint16 count_epaisseur = 0;

        while (current_w > 0 && current_y >= posy) {
            float x_start = posx + ((float)largeur - (float)current_w) / 2.0f;
            tracer_ligne_horizontale(x_start, current_y, current_w, r, g, b);

            current_y -= 1.0f;
            count_epaisseur++;

            if (count_epaisseur >= epaisseur) {
                current_w -= 2;
                count_epaisseur = 0;
            }
        }
    }
    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in draw filled triangle");
}