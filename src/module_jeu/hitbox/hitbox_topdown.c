#include "hitbox.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

/* type a ignorer */
static bool est_type_ignorer_td(int type, int *types_a_ignorer, int nb_types) {
    if (types_a_ignorer == NULL || nb_types <= 0)
        return false;
    for (int i = 0; i < nb_types; i++) {
        if (types_a_ignorer[i] == type)
            return true;
    }
    return false;
}

EntityTopdown *hitbox_topdown(EntityTopdown *entity, Blocks *blocks, int *types_ignorer,
                              int nb_ignorer) {
    float nx = entity->x;
    float ny = entity->y;
    float w = entity->width;
    float h = entity->height;

    for (int i = 0; i < blocks->size; ++i) {
        Block *b = &blocks->tab[i];

        if (est_type_ignorer_td(b->type, types_ignorer, nb_ignorer)) {
            continue;
        }
        float bx = b->x;
        float by = b->y;
        float bw = b->w;
        float bh = b->h;
        if (nx < bx + bw && nx + w > bx && ny < by + bh && ny + h > by) {
            float entity_center_x = nx + w / 2.0f;
            float entity_center_y = ny + h / 2.0f;
            float block_center_x = bx + bw / 2.0f;
            float block_center_y = by + bh / 2.0f;
            float dx = entity_center_x - block_center_x;
            float dy = entity_center_y - block_center_y;
            float half_w_sum = (w / 2.0f) + (bw / 2.0f);
            float half_h_sum = (h / 2.0f) + (bh / 2.0f);
            float overlapX = half_w_sum - fabs(dx);
            float overlapY = half_h_sum - fabs(dy);
            if (overlapX < overlapY) {
                if (dx > 0) {
                    /* L'entité est à droite du bloc */
                    nx = bx + bw;
                } else {
                    /* L'entité est à gauche du bloc */
                    nx = bx - w;
                }
            } else {
                if (dy > 0) {
                    /* L'entité est en bas du bloc */
                    ny = by + bh;
                } else {
                    /* L'entité est en haut du bloc */
                    ny = by - h;
                }
            }
        }
    }
    entity->x = nx;
    entity->y = ny;
    return entity;
}