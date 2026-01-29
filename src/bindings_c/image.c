#include "../moteur/fenetre/fenetre.h"
#include "../moteur/image/ajout/ajout.h"
#include "../moteur/image/caractere/caractere.h"
#include "../moteur/image/chargement/chargement_image.h"
#include "SDL_stdinc.h"

/*
 * Ajoute une image au tableau d'affichage.
 */
void draw(const char *path, float x, float y, Uint16 width, Uint16 height, bool flip,
          Uint16 rotation, Uint8 alpha) {
    ajouter_image_au_tableau(path, x, y, width, height, flip, rotation, alpha);
}

/*
 * Ajoute du texte au tableau d'affichage avec police custom.
 */
void drawText(const char *font_path, const char *text, float x, float y, Uint16 scale, bool flip,
              float spacing, Uint16 rotation, Uint8 alpha) {
    ajouter_mot_dans_tableau(font_path, text, x, y, scale, flip, spacing, rotation, alpha);
}

/*
 * Charge toutes les images d'un dossier en mémoire.
 */
void loadImageFolder(const char *folder) { charger_toutes_les_textures(folder); }

/*
 * Libère toutes les images en mémoire.
 */
void freeImageFolder() { liberer_gestionnaire_image(); }

/*
 * Définit l'icône de la fenêtre.
 */
void setIcon(const char *path) { definir_icone(path); }

/*
 * Dessine un rectangle plein ou vide.
 */
void drawRect(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green, Uint8 blue,
              Uint8 alpha, bool filled) {
    ajouter_rectangle(x, y, width, height, red, green, blue, alpha, filled);
}

/*
 * Dessine un cercle plein ou vide.
 */
void drawCircle(float x, float y, Sint16 radius, Uint8 red, Uint8 green, Uint8 blue, bool filled) {
    ajouter_cercle(x, y, radius, red, green, blue, filled);
}

/*
 * Dessine un triangle plein ou vide.
 */
void drawTriangle(float x, float y, Sint16 w, Sint16 h, Uint8 red, Uint8 green, Uint8 blue,
                  bool filled) {
    ajouter_triangle(x, y, w, h, red, green, blue, filled);
}

/*
 * Dessine une ligne entre deux points.
 */
void drawLine(float x1, float y1, float x2, float y2, Uint8 red, Uint8 green, Uint8 blue) {
    ajouter_ligne(x1, y1, x2, y2, red, green, blue);
}

/* Fonction pour dessiner des sprites */
void drawSprite(Sprite *sprite, Sint16 index, float x, float y, Sint16 w, Sint16 h, bool flip,
                Uint16 rotation, Uint8 alpha) {

    ajouter_sprite_au_tableau(sprite, index, x, y, w, h, flip, rotation, alpha);
}

/* Fonction pour créer un sprite */
Sprite *createSprite(const char *id, Sint16 width, Sint16 height) {
    return creer_sprite(id, width, height);
}
/*dessine des particules*/
void drawParticles(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r, Uint8 *g, Uint8 *b,
                   int size) {
    ajouter_particules_au_tableau(x, y, rotation, a, r, g, b, size);
}