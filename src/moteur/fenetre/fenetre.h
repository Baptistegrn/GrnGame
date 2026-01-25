/*
 * Gestion de la fenêtre et du rendu SDL.
 */

#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gestionnaire Gestionnaire;

typedef struct fondActualiser {
    bool colorier_frame;
    Uint8 r, g, b;
    bool bande_noir;
} fondActualiser;
typedef struct GestionnaireFenetre {
    int largeur_univers;
    int hauteur_univers;
    unsigned char coeff;
    int largeur_actuelle;
    int hauteur_actuelle;
    int decalage_x;
    int decalage_y;
    bool plein_ecran;
    SDL_Window *fenetre;
    SDL_Renderer *rendu;
    const char *nom_fenetre;
} GestionnaireFenetre;

void redimensionner(int w, int h, bool fullscreen_demande);
void definir_icone(const char *chemin);
void afficher_curseur(bool afficher);
void stocker_coloriage(int r, int g, int b);

#ifdef __cplusplus
}
#endif

#endif /* FENETRE_H */
