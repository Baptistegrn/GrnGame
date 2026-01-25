#include "../../main.h"
#include <stdbool.h>

/* type de block ignore de la hitbox */
static bool est_type_ignorer(int type, int *types_a_ignorer, int nb_types) {
    if (types_a_ignorer == NULL || nb_types <= 0)
        return false;
    for (int i = 0; i < nb_types; i++) {
        if (types_a_ignorer[i] == type)
            return true;
    }
    return false;
}

static void renvoie_bloc_hitbox(const Block *bloc, float *bloc_x1, float *bloc_y1, float *bloc_x2,
                                float *bloc_y2) {
    *bloc_x1 = bloc->x;
    *bloc_y1 = bloc->y;
    *bloc_x2 = bloc->x + bloc->w;
    *bloc_y2 = bloc->y + bloc->h;
}

static float calculer_vitesse_saut(float vitesse_y, bool en_air, float force_saut, float gravite,
                                   float dt, float vitesse_max_chute) {
    if (!en_air)
        return 0.0f;

    float nouvelle_vitesse = vitesse_y + gravite * dt;

    if (nouvelle_vitesse > vitesse_max_chute)
        nouvelle_vitesse = vitesse_max_chute;

    return nouvelle_vitesse;
}

static float appliquer_saut(float vitesse_y, bool en_air, float force_saut, bool *nouvel_en_air,
                            bool *saute) {
    if (en_air) {
        *nouvel_en_air = en_air;
        *saute = false;
        return vitesse_y;
    }

    *nouvel_en_air = true;
    *saute = true;
    return force_saut;
}

static bool mettre_a_jour_position(float y, float vitesse_y, float dt, float sol_y,
                                   float hauteur_joueur, float *nouvelle_y,
                                   float *nouvelle_vitesse_y) {
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

static float detecter_sol(float joueur_x1, float joueur_y1, float taillex, float tailley,
                          const Block *blocs, int taille, int *types_ignorer, int nb_ignorer) {
    float sol = 1e9f;
    float joueur_x2 = joueur_x1 + taillex;
    float joueur_y2 = joueur_y1 + tailley;

    for (int i = 0; i < taille; i++) {
        if (est_type_ignorer(blocs[i].type, types_ignorer, nb_ignorer))
            continue;

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

static void detecter_plafond(float joueur_x1, float joueur_y1, float joueur_vitesse_y,
                             const Block *blocs, int taille, float taillex, float tailley,
                             float *nouvelle_y1, float *nouvelle_vitesse_y, int *types_ignorer,
                             int nb_ignorer) {
    float joueur_x2 = joueur_x1 + taillex;
    *nouvelle_y1 = joueur_y1;
    *nouvelle_vitesse_y = joueur_vitesse_y;

    if (joueur_vitesse_y < 0) {
        for (int i = 0; i < taille; i++) {
            if (est_type_ignorer(blocs[i].type, types_ignorer, nb_ignorer))
                continue;

            float bx1, by1, bx2, by2;
            renvoie_bloc_hitbox(&blocs[i], &bx1, &by1, &bx2, &by2);
            if (bx2 > joueur_x1 && bx1 < joueur_x2) {
                if (joueur_y1 < by2 && joueur_y1 > by1) {
                    *nouvelle_vitesse_y = 0.0;
                    *nouvelle_y1 = by2;
                }
            }
        }
    }
}

static void detecter_collisions_murs(float joueur_x1, float joueur_y1, float joueur_vitesse_y,
                                     const Block *blocs, int taille, float taillex, float tailley,
                                     float mur, float dt, float *nouvelle_vitesse_y,
                                     float *nouveau_x, int *types_ignorer, int nb_ignorer,
                                     bool *leftLock, bool *rightLock) {

    *nouvelle_vitesse_y = joueur_vitesse_y;
    *nouveau_x = joueur_x1;

    // Réinitialisation des locks à chaque frame physique
    *leftLock = false;
    *rightLock = false;

    float joueur_x2 = joueur_x1 + taillex;
    float joueur_y2 = joueur_y1 + tailley;

    for (int i = 0; i < taille; i++) {
        if (est_type_ignorer(blocs[i].type, types_ignorer, nb_ignorer))
            continue;

        float bx1, by1, bx2, by2;
        renvoie_bloc_hitbox(&blocs[i], &bx1, &by1, &bx2, &by2);

        if (joueur_y2 > by1 && joueur_y1 < by2) {

            bool contact_mur = false;

            // Collision Droite du joueur (Mur à droite)
            if (joueur_x2 >= bx1 && joueur_x1 < bx1) {
                *nouveau_x = bx1 - taillex;
                *rightLock = true;
                contact_mur = true;
            }
            // Collision Gauche du joueur (Mur à gauche)
            else if (joueur_x1 <= bx2 && joueur_x2 > bx2) {
                *nouveau_x = bx2;
                *leftLock = true;
                contact_mur = true;
            }

            if (contact_mur) {
                // Gestion de la friction (montée et descente)
                if (joueur_vitesse_y > 0) {
                    // Ralentit la chute
                    *nouvelle_vitesse_y -= mur * dt;
                    if (*nouvelle_vitesse_y < 0)
                        *nouvelle_vitesse_y = 0;
                } else if (joueur_vitesse_y < 0) {
                    // Ralentit la montée (friction)
                    *nouvelle_vitesse_y += mur * dt;
                    if (*nouvelle_vitesse_y > 0) {
                        *nouvelle_vitesse_y = 0;
                    }
                }
            }
        }
    }
}

static bool gerer_saut(float vitesse_y, bool en_air, float force_saut, bool a_sauter,
                       float *nouvelle_vy) {
    bool saute, nouvel_en_air;
    if (a_sauter) {
        *nouvelle_vy = appliquer_saut(vitesse_y, en_air, force_saut, &nouvel_en_air, &saute);
        if (saute) {
            return nouvel_en_air;
        }
    }
    *nouvelle_vy = vitesse_y;
    return en_air;
}

void mettre_a_jour_physique_complete(float *x, float *y, float *vitesse_y, bool *en_air,
                                     const Block *blocs, int taille, float dt, float largeur_joueur,
                                     float hauteur_joueur, float gravite, float force_saut,
                                     float vitesse_max_chute, float correction_mur,
                                     int *types_ignorer, int nb_ignorer, bool *leftLock,
                                     bool *rightLock) {

    float sol_y = detecter_sol(*x, *y, largeur_joueur, hauteur_joueur, blocs, taille, types_ignorer,
                               nb_ignorer);

    float nouvelle_vitesse_y =
        calculer_vitesse_saut(*vitesse_y, *en_air, force_saut, gravite, dt, vitesse_max_chute);

    float nouvelle_y;
    bool nouvel_en_air = mettre_a_jour_position(*y, nouvelle_vitesse_y, dt, sol_y, hauteur_joueur,
                                                &nouvelle_y, &nouvelle_vitesse_y);
    float nouveau_x;

    detecter_collisions_murs(*x, *y, nouvelle_vitesse_y, blocs, taille, largeur_joueur,
                             hauteur_joueur, correction_mur, dt, &nouvelle_vitesse_y, &nouveau_x,
                             types_ignorer, nb_ignorer, leftLock, rightLock);

    float nouvelle_y_plafond, nouvelle_vitesse_y_plafond;

    detecter_plafond(nouveau_x, nouvelle_y, nouvelle_vitesse_y, blocs, taille, largeur_joueur,
                     hauteur_joueur, &nouvelle_y_plafond, &nouvelle_vitesse_y_plafond,
                     types_ignorer, nb_ignorer);

    *x = nouveau_x;
    *y = nouvelle_y_plafond;
    *vitesse_y = nouvelle_vitesse_y_plafond;
    *en_air = nouvel_en_air;
}

EntityPlatformer *hitbox_platforme(EntityPlatformer *entite, Blocks *blocs, float vitesse_max_chute,
                                   float correction_mur, int *types_ignorer, int nb_ignorer) {
    entite->inSky = gerer_saut(entite->speedY, entite->inSky, entite->powerJump,
                               entite->requestJump, &entite->speedY);

    float dt_physique = gs->timing->dt;
    mettre_a_jour_physique_complete(&entite->x, &entite->y, &entite->speedY, &entite->inSky,
                                    blocs->tab, blocs->size, dt_physique, entite->width,
                                    entite->height, entite->gravity, entite->powerJump,
                                    vitesse_max_chute, correction_mur, types_ignorer, nb_ignorer,
                                    &entite->leftLock, &entite->rightLock);
    return entite;
}