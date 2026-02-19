/*
 * Gestion de la fenetre et du rendu SDL.
 * Contient les structures de fenetre et les fonctions de rendu.
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

/* Etat du fond a dessiner pour la frame courante */
typedef struct fondActualiser {
    bool colorier_frame;
    Uint8 r, g, b;
    bool bande_noir;
} fondActualiser;

/* Gestionnaire de fenetre et rendu */
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

/* Redimensionnement et mode fenetre */
void passer_plein_ecran(void);
void passer_fenetre_maximisee(void);
void passer_fenetre_taille(int largeur, int hauteur);
void passer_fenetre_coeff(int coeff);
void appliquer_redimensionnement(int largeur_cible, int hauteur_cible);

/* Configuration de la fenetre */
void definir_icone(const char *chemin);
void afficher_curseur(bool afficher);
void stocker_coloriage(int r, int g, int b);

/* Fonctions de liberation */
void liberer_fenetre(void);
void liberer_fond(void);

/* initialise le gestionnaire de fenetrage */
void initialiser_gestionnaire_fenetre(void);

#ifdef __cplusplus
}
#endif

#endif /* FENETRE_H */
