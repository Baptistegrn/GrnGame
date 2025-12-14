#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <math.h>
#include <stdbool.h>


#define TAILLE_LIEN_GT 512

typedef struct Gestionnaire Gestionnaire;

typedef enum { TYPE_IMAGE = 0, TYPE_FORME = 1 } TypeObjet;

// Structure Forme
typedef struct {
  int r, g, b;
  int posx, posy, posx2, posy2, posx3, posy3;
  int plein;
  int type_de_forme;
} Forme;

// Structure Image
typedef struct {
  int posx, posy;
  int taillex, tailley;
  int sens;
  int rotation;
  int transparence;
  SDL_Texture *texture;
} Image;

typedef struct {
  TypeObjet type;
  Image image;
  Forme forme;
} Object_image;

// Tableau dynamique d'images
typedef struct Tableau_image {
  Object_image *tab;
  int nb_images;
  int capacite_images;
} Tableau_image;

// Info frame
typedef struct fond_actualiser {
  bool colorier_frame;
  int r, g, b;
  bool bande_noir;
} fond_actualiser;

// Shaders
typedef struct Shader {
  bool init;
  int *r, *g, *b, *a;
  int *posx, *posy;
  int taille;
} Shader;

// Liste chainée (rotations)
typedef struct Liste {
  SDL_Texture *texture;
  int angle;
  struct Liste *suivant;
} Liste;

// Entrée gestionnaire textures
typedef struct {
  char id[TAILLE_LIEN_GT];
  Liste *textures;
} TextureEntry;

typedef struct GestionnaireTextures {
  TextureEntry *entrees;
  int capacite;
  int taille;
  SDL_Renderer *rendu;
} GestionnaireTextures;

// --- Fonctions d'affichage ---
void afficher_objet(Gestionnaire *gs, Object_image *obj, SDL_Rect *dst,
                    int x_ecran, int y_ecran, int w_ecran, int h_ecran,
                    int coeff);
void afficher_images(Gestionnaire *gs);
void actualiser(Gestionnaire *gs, bool bande_noir);

// --- Caractères ---
void ajouter_mot_dans_tableau(Gestionnaire *gs, char *chemin, const char *mot,
                              int posx, int posy, int coeff, int sens,
                              int ecart, int rotation, int alpha);

// --- Gestionnaire de Textures ---
SDL_Texture *recuperer_texture_par_lien(GestionnaireTextures *gt, char *lien);
void init_gestionnaire_textures(GestionnaireTextures *gt, SDL_Renderer *rendu);
void charger_toutes_les_textures(GestionnaireTextures *gt, const char *dossier);
void liberer_gestionnaire_textures(GestionnaireTextures *gt);
void ajouter_variante_cache(GestionnaireTextures *gt, char *id,
                            SDL_Texture *nouvelle_tex, int angle);
SDL_Texture *recuperer_texture_variante(GestionnaireTextures *gt, char *id,
                                        int angle);
void liberer_gestionnaire_image(GestionnaireTextures *gt);

// ajout au tableau image
void ajouter_image_au_jeu(Gestionnaire *gs, Object_image nouvelle);

// --- Ajout d'objets ---
void ajouter_image_au_tableau(Gestionnaire *gs, char *id, int x, int y, int w,
                              int h, int sens, int rotation, int transparence);

void ajouter_image_au_tableau_batch(Gestionnaire *gs, char **id, int *x, int *y,
                                    int *w, int *h, int *sens, int *rotation,
                                    int *transparence, int taille);

// --- Formes ---
void ajouter_ligne(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                   int g, int b);
void ajouter_rectangle(Gestionnaire *gs, int x, int y, int w, int h, int r,
                       int g, int b, int plein);
void ajouter_cercle(Gestionnaire *gs, int x, int y, int rayon, int r, int g,
                    int b, int plein);
void ajouter_triangle(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                      int g, int b, int plein);

// --- Listes ---
Liste *creer_liste(SDL_Texture *valeur, int angle);
Liste *ajouter_en_tete(Liste *liste, SDL_Texture *valeur, int angle);
Liste *liberer_liste(Liste *liste);
SDL_Texture *recuperer_texture_angle_indice(Liste *liste, int angle);

// --- Shaders ---
void ajouter_shaders(Gestionnaire *gs, int *posx, int *posy, int *r, int *g,
                     int *b, int *a, int taille);
void appliquer_shaders(Gestionnaire *gs);

// bandes noirs
void dessiner_bandes_noires(SDL_Renderer *rendu, int decalage_x, int decalage_y,
                            int largeur, int hauteur);

// --- Coloriage ---
void colorier(Gestionnaire *gs, int r, int g, int b);

// --- Rotation ---
SDL_Texture *creer_texture_tournee_rapide(SDL_Renderer *rendu, SDL_Texture *src,
                                          int w, int h, double angle_deg,
                                          SDL_RendererFlip flip);
void ajuster_rect_texture_tournee(SDL_Texture *texture, SDL_Rect *dst,
                                  int x_ecran, int y_ecran, int w_ecran,
                                  int h_ecran, int coeff);

// --- Primitives de dessin ---
void dessiner_ligne_pixel(Gestionnaire *gs, int x0, int y0, int x1, int y1,
                          int r, int g, int b);
void dessiner_cercle_vide(Gestionnaire *gs, int xc, int yc, int rayon, int r,
                          int g, int b);
void dessiner_cercle_plein(Gestionnaire *gs, int xc, int yc, int rayon, int r,
                           int g, int b);
void dessiner_rectangle_vide(Gestionnaire *gs, int x, int y, int w, int h,
                             int r, int g, int b);
void dessiner_rectangle_plein(Gestionnaire *gs, int x, int y, int w, int h,
                              int r, int g, int b);
void dessiner_triangle_plein(Gestionnaire *gs, int posx, int posy, int largeur,
                             int hauteur, int r, int g, int b);
void dessiner_triangle_vide(Gestionnaire *gs, int posx, int posy, int largeur,
                            int hauteur, int r, int g, int b);

// Primitive pour le dessin
void dessiner_points(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                     int g, int b);
void dessiner_points_n(Gestionnaire *gs, int x1, int y1, int x2, int y2, int r,
                       int g, int b, int n);
void tracer_ligne_horizontale(Gestionnaire *gs, int x_start, int y, int w,
                              int r, int g, int b);
#endif