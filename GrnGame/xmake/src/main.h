#pragma once

// --- INCLUDES ---

// Protection : On n'inclut les librairies C++ que si on compile en C++
#ifdef __cplusplus
    #include "quill/bundled/fmt/base.h"
#endif


#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define TAILLE_LIEN_GT 512

#ifdef __cplusplus
extern "C" {
#endif
#include "image/image.h"
#include "input/input.h"
typedef struct {
    float x;
    float y;
    float w;
    float h;
} Bloc;



typedef struct SonEntry {
    char id[TAILLE_LIEN_GT];
    Mix_Chunk* son;
} SonEntry;

typedef struct GestionnaireSon {
    SonEntry* entrees;
    int taille;
    int capacite;
} GestionnaireSon;

// --- GESTIONNAIRE ENTREES ---



// --- SYSTEME D'IMAGES ET FORMES (NOUVEAU) ---



// --- GESTIONNAIRE PRINCIPAL ---

typedef struct Gestionnaire {
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

    SDL_Texture *shader_vitre;

    fond_actualiser* fond;
    Tableau_image* image;
    Shader *shader;
    GestionnaireEntrees* entrees;
    GestionnaireTextures* textures;
    GestionnaireSon* sons;
    SDL_GameController* controller;
    SDL_Joystick* joystick;
} Gestionnaire;

// --- PROTOTYPES DE FONCTIONS ---

typedef void (*UpdateCallback)(Gestionnaire* jeu);

void set_update_callback(UpdateCallback cb);
Gestionnaire* initialisation(int hauteur, int largeur, float fps, int coeff, char* lien_image, char* lien_son,
                           bool bande_noir, const char* nom_fenetre, const char* chemin_log);
void update(Gestionnaire* jeu);
void boucle_principale(Gestionnaire* jeu);
void liberer_jeu(Gestionnaire* jeu);

int fenetre_init(Gestionnaire* gestionnaire, const char* nom_fenetre);
void redimensionner_fenetre(Gestionnaire* gestionnaire);




// Inputs
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

// Audio
void jouer_son(Gestionnaire* gestionnaire, char* lien, int boucle, int canal, int volume);
void arreter_canal(int canal);
void arreter_son(Gestionnaire* gestionnaire,  char* lien);
void pause_canal(int canal);
void pause_son(Gestionnaire* gestionnaire,  char* lien);
void reprendre_canal(int canal);
void reprendre_son(Gestionnaire* gestionnaire, char* lien);

// Gestionnaires de ressources


Mix_Chunk* recuperer_son_par_lien(GestionnaireSon* gs,  char* lien);
void init_gestionnaire_son(GestionnaireSon* gs);
void charger_tous_les_sons(GestionnaireSon* gs, const char* dossier);
void liberer_gestionnaire_son(GestionnaireSon* gs);

// Utilitaires fichiers
int ends_with_png(const char* name);
int ends_with_wav(const char* name);
void normaliser_chemin(char* chemin);
int collect_pngs(const char* dir, char*** out_list, int* out_count);
int collect_wavs(const char* dir, char*** out_list, int* out_count);

void free_gestionnaire(Gestionnaire* jeu);
void free_tab_images(Gestionnaire* gestionnaire);
void fermer_jeu(Gestionnaire *gs);
void Seticone(Gestionnaire *gs, const char *chemin);

// Physique
void platformer_2d(float dt, float *pos_x, float *pos_y, float *vitesse_y, bool *est_en_air,
                   float larg_joueur, float haut_joueur, const Bloc *blocs, int taille,
                   float gravite, float force_saut,
                   float vitesse_max_chute, float correction_mur,
                   bool activer_saut,
                   bool *collision_gauche, bool *collision_droite);

// Listes chainées

void afficher_curseur(bool x);

#ifdef __cplusplus
}
#endif