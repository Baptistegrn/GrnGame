/*
 * Moteur physique pour les jeux de plateforme.
 * Gere la gravite, le saut, et les collisions avec les blocs.
 */

#include "SDL_stdinc.h"
#include "hitbox.h"
#include <stdbool.h>
#include <stdint.h>

EntityPlatformer *hitbox_platforme(EntityPlatformer *entite, Blocks *blocs, Uint32 masque_ignorer,
                                   float dt) {

    float x = entite->x;
    float y = entite->y;
    float vy = entite->speedY;
    bool en_air = entite->inSky;
    float w = entite->width;
    float h = entite->height;
    float gravite = entite->gravity;
    float force_saut = entite->powerJump;
    bool saut_demande = entite->requestJump;
    bool gauche_demande = entite->requestLeft;
    bool droite_demande = entite->requestRight;
    float vx = entite->speedX;
    float vitesse_max_x = entite->speedMaxX;
    float vitesse_max_chute = entite->speedMaxFall;
    float correction_mur = entite->wallCorrection;
    bool droite_bloque = entite->leftLock;
    bool gauche_bloque = entite->rightLock;

    float nervosite = entite->acceleration;
    float impulsion_depart = entite->initialSpeed;
    /* deplacement gauche */
    if (gauche_demande && !gauche_bloque) {
        if (vx <= 0)
            vx = vitesse_max_x * impulsion_depart;

        vx = (vx >= vitesse_max_x) ? vitesse_max_x : vx + (vitesse_max_x * nervosite) * dt;

        x -= vx;
    }
    /* deplacement droite */
    else if (droite_demande && !droite_bloque) {
        if (vx <= 0)
            vx = vitesse_max_x * impulsion_depart;

        vx = (vx >= vitesse_max_x) ? vitesse_max_x : vx + (vitesse_max_x * nervosite) * dt;

        x += vx;
    }

    /* freinage si arret */
    else {
        vx = (vx <= 0) ? 0 : vx - (vitesse_max_x * nervosite * 1.5f) * dt;
        /* pour camera */
        x = SDL_roundf(x);
        y = SDL_roundf(y);
    }

    int nb_blocs = blocs->size;
    Block *tab_blocs = blocs->tab;

    if (saut_demande && !en_air) {
        vy = force_saut;
        en_air = true;
    }

    float sol_y = 1e9f;
    float x2 = x + w;
    float y2 = y + h;

    float nouv_x = x;
    bool lock_gauche = false;
    bool lock_droite = false;
    bool contact_mur = false;

    for (int i = 0; i < nb_blocs; i++) {
        if ((1U << tab_blocs[i].type) & masque_ignorer)
            continue;

        float bx1 = tab_blocs[i].x;
        float by1 = tab_blocs[i].y;
        float bx2 = bx1 + tab_blocs[i].w;
        float by2 = by1 + tab_blocs[i].h;

        /* detections du sol */
        if (bx2 > x && bx1 < x2) {
            if (by1 >= y2 && by1 < sol_y) {
                sol_y = by1;
            }
        }

        /* detections des murs */
        if (y2 > by1 && y < by2) {
            bool hit_mur = false;

            /* mur a droite */
            if (x2 >= bx1 && x < bx1) {
                nouv_x = bx1 - w;
                lock_droite = true;
                hit_mur = true;
            }
            /* Mur a gauche */
            else if (x <= bx2 && x2 > bx2) {
                nouv_x = bx2;
                lock_gauche = true;
                hit_mur = true;
            }

            if (hit_mur) {
                contact_mur = true;
            }
        }
    }
    /* gravite */
    if (!en_air) {
        vy = 0.0f;
    } else {
        vy += gravite * dt;
        if (vy > vitesse_max_chute) {
            vy = vitesse_max_chute;
        }
    }

    float nouv_y = y + vy * dt;
    bool nouv_en_air = true;

    /* sol */
    if (nouv_y + h >= sol_y) {
        nouv_y = sol_y - h;
        vy = 0.0f;
        nouv_en_air = false;
    }

    /* frictions mur */
    if (contact_mur) {
        if (vy > 0) {
            vy -= correction_mur * dt;
            if (vy < 0)
                vy = 0.0f;
        } else if (vy < 0) {
            vy += correction_mur * dt;
            if (vy > 0)
                vy = 0.0f;
        }
    }

    float fin_y = nouv_y;
    /* verification plafond */
    if (vy < 0) {
        float nouv_x2 = nouv_x + w;

        for (int i = 0; i < nb_blocs; i++) {
            if ((1U << tab_blocs[i].type) & masque_ignorer)
                continue;

            float bx1 = tab_blocs[i].x;
            float by1 = tab_blocs[i].y;
            float bx2 = bx1 + tab_blocs[i].w;
            float by2 = by1 + tab_blocs[i].h;

            if (bx2 > nouv_x && bx1 < nouv_x2) {
                if (fin_y < by2 && fin_y > by1) {
                    vy = 0.0f;
                    fin_y = by2;
                }
            }
        }
    }

    entite->x = nouv_x;
    entite->y = fin_y;
    entite->speedY = vy;
    entite->speedX = vx;
    entite->inSky = nouv_en_air;
    entite->leftLock = lock_gauche;
    entite->rightLock = lock_droite;
    entite->requestJump = false;
    entite->requestLeft = false;
    entite->requestRight = false;

    return entite;
}