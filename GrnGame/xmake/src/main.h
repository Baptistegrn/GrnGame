#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdbool.h>


#define TAILLE_LIEN_GT 512

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    float x;
    float y;
    float w;
    float h;
} Bloc;
typedef struct TextureEntry
{
    char id[TAILLE_LIEN_GT];
    SDL_Texture* texture;
} TextureEntry;

typedef struct GestionnaireTextures
{
    TextureEntry* entrees;
    int taille;
    int capacite;
    SDL_Renderer* rendu;
} GestionnaireTextures;

typedef struct SonEntry
{
    char id[TAILLE_LIEN_GT];
    Mix_Chunk* son;
} SonEntry;

typedef struct GestionnaireSon
{
    SonEntry* entrees;
    int taille;
    int capacite;
} GestionnaireSon;

typedef struct
{
    int x;
    int y;
} StickPos;

typedef struct
{
    StickPos left;
    StickPos right;
} ControllerEtatJoy;

typedef struct
{
    int triggerleft;
    int triggerright;
} Trigger;

typedef struct GestionnaireEntrees
{
    int mouse_x, mouse_y;
    bool mouse_pressed;
    bool mouse_just_pressed;
    bool mouse_right_pressed;
    bool mouse_right_just_pressed;

    bool keys[SDL_NUM_SCANCODES];
    bool keys_pressed[SDL_NUM_SCANCODES];

    bool controller[SDL_CONTROLLER_BUTTON_MAX];
    bool controller_pressed[SDL_CONTROLLER_BUTTON_MAX];
    ControllerEtatJoy Joy;
    Trigger trigger;
} GestionnaireEntrees;

typedef struct
{
    float posx, posy;
    float taillex, tailley;
    int sens;
    int rotation;
    bool forme; 
    int r,g,b;
    int transparence;
    SDL_Texture* texture;
} image;

typedef struct Tableau_image
{
    image* tab;
    int nb_images;
    int capacite_images;
} Tableau_image;

typedef struct fond_actualiser
{
    bool colorier_frame;
    int r,g,b;
    bool bande_noir;
} fond_actualiser;

typedef struct Gestionnaire
{
    bool run;
    float dt;
    float fps;
    int largeur, hauteur;
    int coeff_minimise;
    int largeur_actuel, hauteur_actuel;
    int decalage_x, decalage_y;
    bool plein_ecran;
    Uint32 temps_frame;

    SDL_Window* fenetre;
    SDL_Renderer* rendu;

    fond_actualiser* fond;
    Tableau_image* image;
    GestionnaireEntrees* entrees;
    GestionnaireTextures* textures;
    GestionnaireSon* sons;
    SDL_GameController* controller;
    SDL_Joystick* joystick;
} Gestionnaire;

typedef void (*UpdateCallback)(Gestionnaire* jeu);

void set_update_callback(UpdateCallback cb);
Gestionnaire* initialisation(int hauteur, int largeur, float fps, int coeff, char* lien_image, char* lien_son,
                          bool bande_noir, const char* nom_fenetre, char* chemin_console);
void update(Gestionnaire* jeu);
void boucle_principale(Gestionnaire* jeu);
void liberer_jeu(Gestionnaire* jeu);

int fenetre_init(Gestionnaire* gestionnaire, const char* nom_fenetre);
void redimensionner_fenetre(Gestionnaire* gestionnaire);
void colorier(Gestionnaire* gestionnaire, int r, int g, int b);

void ajouter_image_au_tableau(Gestionnaire* gestionnaire, char* id, float x, float y, float w, float h, int sens,
                              int rotation,int transparence);
void ajouter_forme_au_tableau(Gestionnaire* gestionnaire, float x, float y, float w, float h, int sens,
                              int rotation,int transparence,int r,int g, int b);
void ajouter_forme_au_tableau_batch(Gestionnaire* gestionnaire, float *x, float *y, float *w, float *h, int *sens,
                              int *rotation,int *transparence,int *r,int *g, int *b,int taille);
void ajouter_image_au_tableau_batch(Gestionnaire* gestionnaire,  char** id, float* x, float* y, float* w, float* h,
                                    int* sens, int* rotation,int *transparence, int taille);
void ajouter_mot_dans_tableau(Gestionnaire* jeu,  char* chemin, const char* mot, float posx, float posy,
                              float coeff, int sens, float ecart, int decalage,int alpha);
void actualiser(Gestionnaire* jeu, bool bande_noir);
void afficher_images(Gestionnaire* gestionnaire);

bool touche_juste_presse(Gestionnaire* jeu, const char* touche);
bool touche_enfoncee(Gestionnaire* jeu, const char* touche);
void input_update(Gestionnaire* jeu, GestionnaireEntrees* entrees);
SDL_Scancode scancode_depuis_nom(const char* nom);

bool touche_mannette_enfoncee(Gestionnaire* jeu, const char* touche);
bool touche_mannette_juste_presse(Gestionnaire* jeu, const char* touche);
bool init_controller_joysticks(Gestionnaire* jeu, int index);
void fermer_controller(Gestionnaire* jeu);
void fermer_joystick(Gestionnaire* jeu);
float* renvoie_joysticks(GestionnaireEntrees* entrees, float dead_zone);

void jouer_son(Gestionnaire* gestionnaire,char* lien, int boucle, int canal,int volume);
void arreter_canal(int canal);
void arreter_son(Gestionnaire* gestionnaire,  char* lien);
void pause_canal(int canal);
void pause_son(Gestionnaire* gestionnaire,  char* lien);
void reprendre_canal(int canal);
void reprendre_son(Gestionnaire* gestionnaire, char* lien);

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures* gt,  char* lien);
void init_gestionnaire_textures(GestionnaireTextures* gt, SDL_Renderer* rendu);
void charger_toutes_les_textures(GestionnaireTextures* gt, const char* dossier);
void liberer_gestionnaire_image(GestionnaireTextures* gs);

Mix_Chunk* recuperer_son_par_lien(GestionnaireSon* gs,  char* lien);
void init_gestionnaire_son(GestionnaireSon* gs);
void charger_tous_les_sons(GestionnaireSon* gs, const char* dossier);
void liberer_gestionnaire_son(GestionnaireSon* gs);

int ends_with_png(const char* name);
int ends_with_wav(const char* name);
void normaliser_chemin(char* chemin);
int collect_pngs(const char* dir, char*** out_list, int* out_count);
int collect_wavs(const char* dir, char*** out_list, int* out_count);

void free_gestionnaire(Gestionnaire* jeu);
void free_tab_images(Gestionnaire* gestionnaire);

void ecrire_dans_console(const char* mot);

void Seticone(Gestionnaire *gs,const char *chemin);

void platformer_2d(float dt, float *pos_x, float *pos_y, float *vitesse_y, bool *est_en_air,
                   float larg_joueur, float haut_joueur, const Bloc *blocs, int taille,
                   float gravite, float force_saut,
                   float vitesse_max_chute, float correction_mur,
                   bool activer_saut,
                   bool *collision_gauche, bool *collision_droite);
#ifdef __cplusplus
}
#endif
