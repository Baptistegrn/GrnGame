/*
 * Moteur physique pour les jeux de plateforme .
 * Gere la gravite, le saut, et les collisions avec les blocs.
 */

#include "../../main.h"
#include "../../moteur/logging/logging.h"
#include "../../prediction_branche.h"
#include "../../proprietes.h"
#include "../module_jeu.h"
#include "SDL_stdinc.h"
#include "hitbox.h"
#include <stdbool.h>
#include <stdint.h>

/* gere la hitblox selon une liste de blocs et dentites */
void hitbox_platforme(EntitePlatformes *entites, Blocs *blocs, Uint32 masque_ignorer, float dt) {

    for (int i = 0; i < entites->taille; i++) {
        EntitePlatforme *entite = &(entites->entites[i]);
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
        bool gauche_bloque = entite->leftLock;
        bool droite_bloque = entite->rightLock;
        float nervosite = entite->acceleration;
        float impulsion_depart = entite->initialSpeed;
        int nb_sauts = entite->numberOfJumps;
        bool saut_sur_murs = entite->jumpOnWall;
        int nb_saut_max = entite->numberOfJumpsPossible;

        /* Chargement et mise à jour des timers de murs */
        float timer_mur_gauche = entite->wallTimerLeft - dt;
        float timer_mur_droite = entite->wallTimerRight - dt;

        /* Gestion du Coyote Time au sol */
        float timer_sol = entite->groundTimer;
        if (en_air) {
            timer_sol -= dt;
        } else {
            timer_sol = TOLERANCE_SOL;
        }

        /* Gestion du blocage directionnel */
        float timer_blocage = entite->wallJumpLockTimer;
        if (timer_blocage > 0) {
            timer_blocage -= dt;
            if (timer_blocage < 0)
                timer_blocage = 0;
            /* Force le deplacement à droite, ignore les inputs du joueur */
            droite_demande = true;
            gauche_demande = false;
        } else if (timer_blocage < 0) {
            timer_blocage += dt;
            if (timer_blocage > 0)
                timer_blocage = 0;
            /* Force le deplacement à gauche, ignore les inputs du joueur */
            gauche_demande = true;
            droite_demande = false;
        }

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
        Bloc *tab_blocs = blocs->tab;

        /* saut (mur et sol) */
        if (saut_demande) {
            if (saut_sur_murs && en_air &&
                ((timer_mur_gauche > 0 && droite_demande) ||
                 (timer_mur_droite > 0 && gauche_demande))) {
                /* Saut mural : on saute et on se repousse du mur */
                vy = force_saut;

                if (timer_mur_gauche > 0 && droite_demande) {
                    x += 4.0f;          /* Pousse le joueur vers la droite */
                    vx = vitesse_max_x; /* Redonne de l'elan horizontal */
                    timer_mur_gauche = 0.0f;
                    timer_blocage = DUREE_BLOCAGE_MURAL; /* Propulse de force vers la droite */
                } else if (timer_mur_droite > 0 && gauche_demande) {
                    x -= 4.0f; /* Pousse le joueur vers la gauche */
                    vx = vitesse_max_x;
                    timer_mur_droite = 0.0f;
                    timer_blocage = -DUREE_BLOCAGE_MURAL; /* Propulse de force vers la gauche */
                }
            } else if (!en_air || timer_sol > 0) {
                /* Saut normal depuis le sol (ou via Coyote Time du sol) */
                vy = force_saut;
                en_air = true;
                timer_sol = 0.0f; /* Consomme le timer sol pour eviter de resauter de suite */
                if (nb_sauts > 0) {
                    nb_sauts--;
                }
            } else if (nb_sauts > 0) {
                /* Double saut (en l'air mais pas sur un mur ni juste apres avoir quitte le sol) */
                vy = force_saut;
                nb_sauts--;
            }
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
                    timer_mur_droite = TOLERANCE_MUR;
                }
                /* Mur a gauche */
                else if (x <= bx2 && x2 > bx2) {
                    nouv_x = bx2;
                    lock_gauche = true;
                    hit_mur = true;
                    timer_mur_gauche = TOLERANCE_MUR;
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
            nb_sauts = nb_saut_max;
            timer_sol = TOLERANCE_SOL;
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
        entite->numberOfJumps = nb_sauts;
        entite->wallTimerLeft = timer_mur_gauche;
        entite->wallTimerRight = timer_mur_droite;
        entite->groundTimer = timer_sol;
        entite->wallJumpLockTimer = timer_blocage;
    }
}

/* Fonction qui appelle le thread */
void lancer_calcul_hitbox(EntitePlatformes *entites, Blocs *blocs, Uint32 masque, float dt) {
    if (UNLIKELY(!gs))
        goto gsvide;

    if (UNLIKELY(!gs->module_jeu->thread_present)) {
        /* si thread non present on fait sur le main thread */
        hitbox_platforme(entites, blocs, masque, dt);
        return;
    }

    SDL_LockMutex(gs->module_jeu->mutex);

    gs->module_jeu->donnees.entites = entites;
    gs->module_jeu->donnees.blocs = blocs;
    gs->module_jeu->donnees.masque_ignorer = masque;
    gs->module_jeu->donnees.dt = dt;

    gs->module_jeu->travail_demande = true;
    gs->module_jeu->travail_termine = false;
    SDL_CondSignal(gs->module_jeu->reveil);
    SDL_UnlockMutex(gs->module_jeu->mutex);
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in hitbox function");
}
