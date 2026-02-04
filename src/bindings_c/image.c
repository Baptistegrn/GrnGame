/*
 * Bindings C pour le rendu graphique.
 * Expose les fonctions de dessin d'images, formes et textes.
 */

#include "../moteur/fenetre/fenetre.h"
#include "../moteur/image/ajout/ajout.h"
#include "../moteur/image/caractere/caractere.h"
#include "../moteur/image/chargement/chargement_image.h"
#include "SDL_stdinc.h"

/* Ajoute une image au tableau d'affichage */
void draw(const char *path, float x, float y, Uint16 width, Uint16 height, bool flip,
          Uint16 rotationP, Uint16 rotation, Uint8 alpha) {
    ajouter_image_au_tableau(path, x, y, width, height, flip, rotationP, rotation, alpha);
}

/* Ajoute du texte au tableau d'affichage avec police custom */
void drawText(const char *font_path, const char *text, float x, float y, Uint16 scale, bool flip,
              float spacing, Uint16 rotationP, Uint16 rotation, Uint8 alpha) {
    ajouter_mot_dans_tableau(font_path, text, x, y, scale, flip, spacing, rotationP, rotation,
                             alpha);
}

/* Charge toutes les images d'un dossier en memoire */
void loadImageFolder(const char *folder) { charger_toutes_les_textures(folder); }

/* Libere toutes les images en memoire */
void freeImageFolder(void) { liberer_gestionnaire_image(); }

/* Definit l'icone de la fenetre */
void setIcon(const char *path) { definir_icone(path); }

/* Dessine un rectangle plein ou vide */
void drawRect(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green, Uint8 blue,
              Uint8 alpha, bool filled) {
    ajouter_rectangle(x, y, width, height, red, green, blue, alpha, filled);
}

/* Dessine un cercle plein ou vide */
void drawCircle(float x, float y, Sint16 radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha,
                bool filled) {
    ajouter_cercle(x, y, radius, red, green, blue, alpha, filled);
}

/* Dessine un triangle plein ou vide */
void drawTriangle(float x, float y, Sint16 w, Sint16 h, Uint8 red, Uint8 green, Uint8 blue,
                  Uint8 alpha, bool filled) {
    ajouter_triangle(x, y, w, h, red, green, blue, alpha, filled);
}

/* Dessine une ligne entre deux points */
void drawLine(float x1, float y1, float x2, float y2, Uint8 red, Uint8 green, Uint8 blue,
              Uint8 alpha) {
    ajouter_ligne(x1, y1, x2, y2, red, green, blue, alpha);
}

/* Dessine un sprite a la position indiquee */
void drawSprite(Sprite *sprite, Sint16 index, float x, float y, Sint16 w, Sint16 h, bool flip,
                Uint16 rotationP, Uint16 rotation, Uint8 alpha) {
    ajouter_sprite_au_tableau(sprite, index, x, y, w, h, flip, rotationP, rotation, alpha);
}

/* Cree un sprite a partir d'un identifiant d'image */
Sprite *createSprite(const char *id, Sint16 width, Sint16 height) {
    return creer_sprite(id, width, height);
}

/* Dessine un tableau de particules */
void drawParticles(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r, Uint8 *g, Uint8 *b,
                   int size) {
    ajouter_particules_au_tableau(x, y, rotation, a, r, g, b, size);
}