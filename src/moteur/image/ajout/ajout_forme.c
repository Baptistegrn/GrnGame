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

    /* positions camera */
    float ecran_x1 = x1 - gs->camera->x;
    float ecran_y1 = y1 - gs->camera->y;
    float ecran_x2 = x2 - gs->camera->x;
    float ecran_y2 = y2 - gs->camera->y;

    float min_x = (ecran_x1 < ecran_x2) ? ecran_x1 : ecran_x2;
    float min_y = (ecran_y1 < ecran_y2) ? ecran_y1 : ecran_y2;
    int largeur = (int)fabs(ecran_x2 - ecran_x1);
    int hauteur = (int)fabs(ecran_y2 - ecran_y1);

    int decalage_x = (int)lround((double)gs->fenetre->decalage_x / (double)gs->fenetre->coeff);
    int decalage_y = (int)lround((double)gs->fenetre->decalage_y / (double)gs->fenetre->coeff);

    if (hors_ecran(min_x, min_y, largeur, hauteur, decalage_x, decalage_y)) {
        return;
    }

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_FORME;
    tab->tab[i].forme.type_de_forme = 1;

    tab->tab[i].forme.posx = ecran_x1;
    tab->tab[i].forme.posy = ecran_y1;
    tab->tab[i].forme.posx2 = ecran_x2;
    tab->tab[i].forme.posy2 = ecran_y2;

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

    int decalage_x = (int)lround((double)gs->fenetre->decalage_x / (double)gs->fenetre->coeff);
    int decalage_y = (int)lround((double)gs->fenetre->decalage_y / (double)gs->fenetre->coeff);

    /* positions camera */
    float ecran_x = x - gs->camera->x;
    float ecran_y = y - gs->camera->y;

    if (hors_ecran(ecran_x, ecran_y, taillex, tailley, decalage_x, decalage_y)) {
        return;
    }

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_FORME;
    tab->tab[i].forme.type_de_forme = 2;
    tab->tab[i].forme.posx = ecran_x;
    tab->tab[i].forme.posy = ecran_y;
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

    /* positions camera */
    float ecran_x = x - gs->camera->x;
    float ecran_y = y - gs->camera->y;

    int decalage_x = (int)lround((double)gs->fenetre->decalage_x / (double)gs->fenetre->coeff);
    int decalage_y = (int)lround((double)gs->fenetre->decalage_y / (double)gs->fenetre->coeff);

    if (hors_ecran(ecran_x - rayon, ecran_y - rayon, rayon * 2, rayon * 2, decalage_x,
                   decalage_y)) {
        return;
    }

    TableauImage *tab = gs->frame->image;
    reallouer_si_plein();
    int i = tab->nb_images++;

    tab->tab[i].type = TYPE_FORME;
    tab->tab[i].forme.type_de_forme = 3;

    tab->tab[i].forme.posx = ecran_x;
    tab->tab[i].forme.posy = ecran_y;
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