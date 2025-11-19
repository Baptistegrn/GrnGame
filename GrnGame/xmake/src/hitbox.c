#include "main.h"

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdbool.h>



void renvoie_bloc_hitbox(const Bloc *bloc,
                         float *bloc_x1, float *bloc_y1,
                         float *bloc_x2, float *bloc_y2)
{
    *bloc_x1 = bloc->x;
    *bloc_y1 = bloc->y;
    *bloc_x2 = bloc->x + bloc->w;
    *bloc_y2 = bloc->y + bloc->h;
}


float calculer_vitesse_saut(float vitesse_y, bool en_air,
                            float force_saut, float gravite,
                            float dt, float vitesse_max_chute)
{
    if (!en_air)
        return 0.0f;

    float nouvelle_vitesse = vitesse_y + gravite * dt;

    if (nouvelle_vitesse > vitesse_max_chute)
        nouvelle_vitesse = vitesse_max_chute;

    return nouvelle_vitesse;
}


float appliquer_saut(float vitesse_y, bool en_air, float force_saut,
                     bool *nouvel_en_air, bool *saute)
{
    if (en_air) {
        *nouvel_en_air = en_air;
        *saute = false;
        return vitesse_y;
    }

    *nouvel_en_air = true;
    *saute = true;
    return force_saut;
}


bool mettre_a_jour_position(float y, float vitesse_y, float dt,
                            float sol_y, float hauteur_joueur,
                            float *nouvelle_y, float *nouvelle_vitesse_y)
{
    *nouvelle_y = y + vitesse_y * dt;
    *nouvelle_vitesse_y = vitesse_y;

    bool nouvel_en_air = true;

    if (*nouvelle_y + hauteur_joueur >= sol_y) {
        *nouvelle_y = sol_y - hauteur_joueur;
        *nouvelle_vitesse_y = 0.0f;
        nouvel_en_air = false;
    }

    return nouvel_en_air;
}


float detecter_sol(float joueur_x1, float joueur_y1,
                   float taillex, float tailley,
                   const Bloc *blocs, int taille)
{
    float sol = 1e9f;

    float joueur_x2 = joueur_x1 + taillex;
    float joueur_y2 = joueur_y1 + tailley;

    for (int i = 0; i < taille; i++) {

        float bx1, by1, bx2, by2;
        renvoie_bloc_hitbox(&blocs[i], &bx1, &by1, &bx2, &by2);

        if (bx2 > joueur_x1 && bx1 < joueur_x2) {
            if (by1 >= joueur_y2 && by1 < sol) {
                sol = by1;
            }
        }
    }

    return sol;
}

void detecter_plafond(float joueur_x1,float joueur_y1,float joueur_vitesse_y,const Bloc *blocs ,int taille,float taillex,float tailley, float *nouvelle_y1,float *nouvelle_vitesse_y){
   float joueur_x2 = joueur_x1+taillex;
   *nouvelle_y1 = joueur_y1;
   *nouvelle_vitesse_y = joueur_vitesse_y;
    if(joueur_vitesse_y<0){
    for (int i = 0; i < taille; i++){
        float bx1, by1, bx2, by2;
        renvoie_bloc_hitbox(&blocs[i], &bx1, &by1, &bx2, &by2);
        if(bx2>joueur_x1 && bx1<joueur_x2){
            if(joueur_y1<by2 && joueur_y1>by1){
                *nouvelle_vitesse_y = 0.0;
                *nouvelle_y1 = by2;
            }
        }
    }
    }
}

void detecter_collisions_murs(float joueur_x1, float joueur_y1,
                              float joueur_vitesse_y, const Bloc *blocs,
                              int taille, float taillex, float tailley,
                              float mur, float dt,
                              bool *collision_gauche, bool *collision_droite,
                              float *nouvelle_vitesse_y, float *nouveau_x)
{
    *collision_gauche = false;
    *collision_droite = false;
    *nouvelle_vitesse_y = joueur_vitesse_y;
    *nouveau_x = joueur_x1;

    float joueur_x2 = joueur_x1 + taillex;
    float joueur_y2 = joueur_y1 + tailley;

    for (int i = 0; i < taille; i++) {
        float bx1, by1, bx2, by2;
        renvoie_bloc_hitbox(&blocs[i], &bx1, &by1, &bx2, &by2);


        if (joueur_y2 > by1 && joueur_y1 < by2) {

            if (joueur_x2 >= bx1 && joueur_x1 < bx1) {
                *collision_droite = true;
                *nouveau_x = bx1 - taillex;
                if (joueur_vitesse_y < 0)
                    *nouvelle_vitesse_y += mur * dt;
            }

            else if (joueur_x1 <= bx2 && joueur_x2 > bx2) {
                *collision_gauche = true;
                *nouveau_x = bx2;
                if (joueur_vitesse_y < 0)
                    *nouvelle_vitesse_y += mur * dt;
            }
        }
    }
}


bool gerer_saut(float vitesse_y,bool en_air,float force_saut,bool a_sauter,float *nouvelle_vy){
    bool saute,nouvel_en_air;
    if (a_sauter){
        *nouvelle_vy = appliquer_saut(vitesse_y,en_air, force_saut,&nouvel_en_air,&saute);
        if(saute){
            return nouvel_en_air;
        }

    }
    *nouvelle_vy =vitesse_y;
    return en_air;
}


void mettre_a_jour_physique_complete(float *x, float *y, float *vitesse_y, bool *en_air,
                                     const Bloc *blocs, int taille,
                                     float dt, float largeur_joueur, float hauteur_joueur,
                                     float gravite, float force_saut,
                                     float vitesse_max_chute, float correction_mur,
                                     bool *collision_gauche, bool *collision_droite)
{
    float sol_y = detecter_sol(*x, *y, largeur_joueur, hauteur_joueur, blocs, taille);
    float nouvelle_vitesse_y = calculer_vitesse_saut(*vitesse_y, *en_air, force_saut, gravite, dt, vitesse_max_chute);

    float nouvelle_y;
    bool nouvel_en_air = mettre_a_jour_position(*y, nouvelle_vitesse_y, dt, sol_y, hauteur_joueur,
                                                &nouvelle_y, &nouvelle_vitesse_y);
    float nouveau_x;
    detecter_collisions_murs(*x, *y, nouvelle_vitesse_y, blocs, taille,
                             largeur_joueur, hauteur_joueur, correction_mur, dt,
                             collision_gauche, collision_droite,
                             &nouvelle_vitesse_y, &nouveau_x);

    float nouvelle_y_plafond, nouvelle_vitesse_y_plafond;
    detecter_plafond(nouveau_x, nouvelle_y, nouvelle_vitesse_y, blocs, taille,
                     largeur_joueur, hauteur_joueur,
                     &nouvelle_y_plafond, &nouvelle_vitesse_y_plafond);


    *x = nouveau_x;
    *y = nouvelle_y_plafond;
    *vitesse_y = nouvelle_vitesse_y_plafond;
    *en_air = nouvel_en_air;
}


void platformer_2d(float dt, float *pos_x, float *pos_y, float *vitesse_y, bool *est_en_air,
                   float larg_joueur, float haut_joueur, const Bloc *blocs, int taille,
                   float gravite, float force_saut,
                   float vitesse_max_chute, float correction_mur,
                   bool activer_saut,
                   bool *collision_gauche, bool *collision_droite)
{
    *est_en_air = gerer_saut(*vitesse_y, *est_en_air, force_saut, activer_saut, vitesse_y);

    mettre_a_jour_physique_complete(pos_x, pos_y, vitesse_y, est_en_air, blocs, taille,
                                    dt, larg_joueur, haut_joueur,
                                    gravite, force_saut, vitesse_max_chute, correction_mur,
                                    collision_gauche, collision_droite);
}