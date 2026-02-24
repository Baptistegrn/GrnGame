/*
 * Affichage optimise des particules avec rendu GPU.
 */

#include "../../../main.h"
#include "../../../module_jeu/module_jeu.h"
#include "../../../prediction_branche.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "affichage.h"

#ifdef _WIN32
#include <malloc.h> /* bug alloc windows */
#endif

/* tableaux statiques pour eviter allocations */
/* TODO : deplacer vers gestionnaireFrame */
static SDL_Vertex verts[MAX_PARTICULES * 4];
static int indices[MAX_PARTICULES * 6];

/* Dessine les particules de maniere optimisee avec du rendu gpu */
void dessiner_particules(Particule *particules) {
    if (UNLIKELY(!gs))
        goto gsvide;

    if (UNLIKELY(!particules || particules->taille <= 0)) {
        log_message(NiveauLogErreur, "Size of particles equals 0 or null pointer");
        return;
    }

    float coeff = (float)gs->fenetre->coeff;
    float decalage_x = (float)gs->fenetre->decalage_x;
    float decalage_y = (float)gs->fenetre->decalage_y;
    /* limite */
    int max = (particules->taille > MAX_PARTICULES) ? MAX_PARTICULES : particules->taille;

    /* reset du batch */
    int vcount = 0;
    int icount = 0;

    /* ordre de construction des triangles */
    int base_indices[6] = {0, 1, 2, 2, 3, 0};

    for (int i = 0; i < max; i++) {
        /* positions ecran */
        /* ajout ici de la camera */
        int x = lroundf((particules->posx[i] - gs->module_jeu->camera->x) * coeff + decalage_x);
        int y = lroundf((particules->posy[i] - gs->module_jeu->camera->y) * coeff + decalage_y);

        /* taille = 1 pixel (scale) */
        int w = coeff;
        int h = coeff;

        float angle = (float)particules->rotation[i];

        /* centre du rectangle */
        float cx = x + w * 0.5f;
        float cy = y + h * 0.5f;

        /* coins avant rotation */
        SDL_FPoint pts[4] = {{x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}};

        float c = cosf(angle);
        float s = sinf(angle);

        SDL_Color color = {particules->r[i], particules->g[i], particules->b[i], particules->a[i]};

        /* index de depart pour cette particule */
        int base = vcount;

        for (int j = 0; j < 4; j++) {
            float dx = pts[j].x - cx;
            float dy = pts[j].y - cy;

            verts[vcount].position.x = cx + dx * c - dy * s;
            verts[vcount].position.y = cy + dx * s + dy * c;

            verts[vcount].color = color;
            verts[vcount].tex_coord = (SDL_FPoint){0, 0};

            vcount++;
        }

        /* indices du carre (relatifs a base) */
        indices[icount++] = base + base_indices[0];
        indices[icount++] = base + base_indices[1];
        indices[icount++] = base + base_indices[2];
        indices[icount++] = base + base_indices[3];
        indices[icount++] = base + base_indices[4];
        indices[icount++] = base + base_indices[5];
    }

    /* rendu batch */
    if (LIKELY(vcount > 0 && icount > 0)) {
        SDL_RenderGeometry(gs->fenetre->rendu, NULL, verts, vcount, indices, icount);
    }
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in draw particles function");
}