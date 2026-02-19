/*
 * Ajout de formes geometriques a la liste de rendu.
 */

#include "../../../main.h"
#include "../../../module_jeu/camera/camera.h"
#include "../../boucle/boucle.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"
#include "../affichage/affichage.h"
#include "ajout.h"
#include <math.h>
#include <stdbool.h>

/* Ajoute une ligne a la liste de rendu */
void ajouter_ligne(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (!gs)
        goto gsvide;

    /* Culling : Calcul de la boite englobante de la ligne */
    float min_x = (x1 < x2) ? x1 : x2;
    float min_y = (y1 < y2) ? y1 : y2;
    int largeur = (int)fabs(x2 - x1);
    int hauteur = (int)fabs(y2 - y1);

    if (hors_ecran(min_x, min_y, largeur, hauteur, gs->fenetre->decalage_x,
                   gs->fenetre->decalage_y)) {
        return;
    }

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_FORME;
    tab->tab[i].forme.type_de_forme = 1;
    tab->tab[i].forme.posx = x1 - gs->camera->x;
    tab->tab[i].forme.posy = y1 - gs->camera->y;

    tab->tab[i].forme.posx2 = x2 - gs->camera->x;
    tab->tab[i].forme.posy2 = y2 - gs->camera->y;

    tab->tab[i].forme.r = SDL_clamp(r, 0, 255);
    tab->tab[i].forme.g = SDL_clamp(g, 0, 255);
    tab->tab[i].forme.b = SDL_clamp(b, 0, 255);
    tab->tab[i].forme.a = SDL_clamp(a, 0, 255);
    tab->tab[i].forme.plein = true;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add line function");
}

/* Ajoute un rectangle a la liste de rendu */
void ajouter_rectangle(float x, float y, Sint16 taillex, Sint16 tailley, Uint8 r, Uint8 g, Uint8 b,
                       Uint8 a, bool plein) {
    if (!gs)
        goto gsvide;

    /* Culling basique (le plus rapide) */
    if (hors_ecran(x, y, taillex, tailley, gs->fenetre->decalage_x, gs->fenetre->decalage_y)) {
        return;
    }

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_FORME;
    tab->tab[i].forme.type_de_forme = 2;
    tab->tab[i].forme.posx = x - gs->camera->x;
    tab->tab[i].forme.posy = y - gs->camera->y;
    tab->tab[i].forme.taillex = taillex;
    tab->tab[i].forme.tailley = tailley;
    tab->tab[i].forme.r = SDL_clamp(r, 0, 255);
    tab->tab[i].forme.g = SDL_clamp(g, 0, 255);
    tab->tab[i].forme.b = SDL_clamp(b, 0, 255);
    tab->tab[i].forme.a = SDL_clamp(a, 0, 255);
    tab->tab[i].forme.plein = plein;

    return;

gsvide:
    log_message(NiveauLogDebug, "Debug : manager is empty in add rectangle function");
}

/* Ajoute un cercle a la liste de rendu */
void ajouter_cercle(float x, float y, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    bool plein) {
    if (!gs)
        goto gsvide;

    /* Culling : On recule x et y du rayon pour tester le "carre" qui englobe le cercle */
    if (hors_ecran(x - rayon, y - rayon, rayon * 2, rayon * 2, gs->fenetre->decalage_x,
                   gs->fenetre->decalage_y)) {
        return;
    }

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_FORME;
    tab->tab[i].forme.type_de_forme = 3;
    tab->tab[i].forme.posx = x - gs->camera->x;
    tab->tab[i].forme.posy = y - gs->camera->y;
    tab->tab[i].forme.taillex = rayon * 2;
    tab->tab[i].forme.tailley = rayon * 2;
    tab->tab[i].forme.r = SDL_clamp(r, 0, 255);
    tab->tab[i].forme.g = SDL_clamp(g, 0, 255);
    tab->tab[i].forme.b = SDL_clamp(b, 0, 255);
    tab->tab[i].forme.a = SDL_clamp(a, 0, 255);
    tab->tab[i].forme.plein = plein;

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add circle function");
}