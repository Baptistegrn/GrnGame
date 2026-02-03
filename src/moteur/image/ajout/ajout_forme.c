/*
 * Ajout de formes geometriques a la liste de rendu.
 */

#include "../../../main.h"
#include "ajout.h"

/* Ajoute une ligne a la liste de rendu */
void ajouter_ligne(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b) {
    if (!gs)
        goto gsvide;
    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs RGB entre 0 et 255 */
    r = SDL_clamp(r, 0, 255);
    g = SDL_clamp(g, 0, 255);
    b = SDL_clamp(b, 0, 255);

    obj.type = TYPE_FORME;
    obj.forme.type_de_forme = 1;
    obj.forme.posx = x1;
    obj.forme.posy = y1;
    obj.forme.taillex = x2;
    obj.forme.tailley = y2;
    obj.forme.r = r;
    obj.forme.g = g;
    obj.forme.b = b;
    obj.forme.plein = true;

    ajouter_image_au_jeu(obj);

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add line function");
}

/* Ajoute un rectangle a la liste de rendu */
void ajouter_rectangle(float x, float y, Sint16 taillex, Sint16 tailley, Uint8 r, Uint8 g, Uint8 b,
                       Uint8 a, bool plein) {
    if (!gs)
        goto gsvide;
    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs RGB entre 0 et 255 */
    r = SDL_clamp(r, 0, 255);
    g = SDL_clamp(g, 0, 255);
    b = SDL_clamp(b, 0, 255);
    a = SDL_clamp(a, 0, 255);
    obj.type = TYPE_FORME;
    obj.forme.type_de_forme = 2;
    obj.forme.posx = x;
    obj.forme.posy = y;
    obj.forme.taillex = taillex;
    obj.forme.tailley = tailley;
    obj.forme.r = r;
    obj.forme.g = g;
    obj.forme.b = b;
    obj.forme.a = a;
    obj.forme.plein = plein;

    ajouter_image_au_jeu(obj);

    return;

gsvide:
    log_message(NiveauLogDebug, "Debug : manager is empty in add rectangle function");
}

/* Ajoute un cercle a la liste de rendu */
void ajouter_cercle(float x, float y, Sint16 rayon, Uint8 r, Uint8 g, Uint8 b, bool plein) {
    if (!gs)
        goto gsvide;
    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs RGB entre 0 et 255 */
    r = SDL_clamp(r, 0, 255);
    g = SDL_clamp(g, 0, 255);
    b = SDL_clamp(b, 0, 255);

    obj.type = TYPE_FORME;
    obj.forme.type_de_forme = 3;
    obj.forme.posx = x;
    obj.forme.posy = y;
    obj.forme.taillex = rayon * 2;
    obj.forme.tailley = rayon * 2;
    obj.forme.r = r;
    obj.forme.g = g;
    obj.forme.b = b;
    obj.forme.plein = plein;

    ajouter_image_au_jeu(obj);

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add circle function");
}

/* Ajoute un triangle a la liste de rendu */
void ajouter_triangle(float x, float y, Sint16 taillex, Sint16 tailley, Uint8 r, Uint8 g, Uint8 b,
                      bool plein) {
    if (!gs)
        goto gsvide;
    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs RGB entre 0 et 255 */
    r = SDL_clamp(r, 0, 255);
    g = SDL_clamp(g, 0, 255);
    b = SDL_clamp(b, 0, 255);

    obj.type = TYPE_FORME;
    obj.forme.type_de_forme = 4;
    obj.forme.posx = x;
    obj.forme.posy = y;
    obj.forme.taillex = taillex;
    obj.forme.tailley = tailley;
    obj.forme.r = r;
    obj.forme.g = g;
    obj.forme.b = b;
    obj.forme.plein = plein;

    ajouter_image_au_jeu(obj);

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add triangle function");
}