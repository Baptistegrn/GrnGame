/*
 * Moteur physique pour les jeux vue de dessus (top-down).
 * Gere les collisions AABB avec resolution par axe minimal.
 */

#include "hitbox.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * Calcule les collisions pour une entite vue de dessus.
 * Resout les collisions en poussant l'entite sur l'axe de moindre penetration.
 */
EntityTopdown *hitbox_topdown(EntityTopdown *entity, Blocks *blocks, int *types_ignorer,
                              int nb_ignorer) {
    float nx = entity->x;
    float ny = entity->y;
    float w = entity->width;
    float h = entity->height;

    for (int i = 0; i < blocks->size; ++i) {
        Block *b = &blocks->tab[i];

        if (est_type_ignorer(b->type, types_ignorer, nb_ignorer)) {
            continue;
        }

        float bx = b->x;
        float by = b->y;
        float bw = b->w;
        float bh = b->h;

        /* Test de collision AABB */
        if (nx < bx + bw && nx + w > bx && ny < by + bh && ny + h > by) {
            /* Calcul des centres */
            float entity_center_x = nx + w / 2.0f;
            float entity_center_y = ny + h / 2.0f;
            float block_center_x = bx + bw / 2.0f;
            float block_center_y = by + bh / 2.0f;

            /* Distance entre centres */
            float dx = entity_center_x - block_center_x;
            float dy = entity_center_y - block_center_y;

            /* Chevauchement sur chaque axe */
            float half_w_sum = (w / 2.0f) + (bw / 2.0f);
            float half_h_sum = (h / 2.0f) + (bh / 2.0f);
            float overlapX = half_w_sum - fabs(dx);
            float overlapY = half_h_sum - fabs(dy);

            /* Resolution sur l'axe de moindre penetration */
            if (overlapX < overlapY) {
                if (dx > 0) {
                    /* Entite a droite du bloc */
                    nx = bx + bw;
                } else {
                    /* Entite a gauche du bloc */
                    nx = bx - w;
                }
            } else {
                if (dy > 0) {
                    /* Entite en bas du bloc */
                    ny = by + bh;
                } else {
                    /* Entite en haut du bloc */
                    ny = by - h;
                }
            }
        }
    }

    entity->x = nx;
    entity->y = ny;
    return entity;
}