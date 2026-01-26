/*
 * API C du moteur GrnGame
 * Expose toutes les fonctionnalités du moteur graphique, audio et input
 */

#ifndef GRNGAME_H
#define GRNGAME_H
#include "../chemin/chemin.h"
#include <SDL_stdinc.h>
#include <stdbool.h>

#if defined(_WIN32)
#if defined(GAME_EXPORTS)
#define GRN_API __declspec(dllexport)
#else
#define GRN_API __declspec(dllimport)
#endif
#else
/* Linux / Mac */
#define GRN_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 } LogLevel;

typedef struct {
    float x;
    float y;
    float w;
    float h;
    int type;
} Block;

typedef struct {
    Block *tab;
    int size;
    int capacity;
} Blocks;
typedef struct {
    float x;
    float y;
    float width;
    float height;
} EntityTopdown;

typedef struct {
    float x;
    float y;
    float speedY;
    float width;
    float height;
    float gravity;
    float powerJump;
    bool inSky;
    bool requestJump;
    bool leftLock;
    bool rightLock;
} EntityPlatformer;

typedef struct {
    float x;
    float y;
    float smooth_factor;
    int width;
    int height;
} Camera;

typedef void (*UpdateCallback)(void);

/*initialisation de la fenetre*/
GRN_API void initialize(int heightU, int widthU, float fps_target, int black_bars,
                        const char *window_title, UpdateCallback update_func);

/* Arrête le moteur et ferme l'application */
GRN_API void stop(void);

/* Redimensionne la fenêtre et ajuste le mode plein écran */
GRN_API void resize(int w, int h, bool fullscreen_requested, bool window_fullscreen_requested);

/* Efface l'écran avec la couleur spécifiée */
GRN_API void clearScreen(int red, int green, int blue);

/* Calcule la physique de plateforme 2D avec gestion des collisions */
GRN_API EntityPlatformer *hitboxPlatformer(EntityPlatformer *entite, Blocks *blocs,
                                           float vitesse_max_chute, float correction_mur,
                                           int *ignored_type, int size);

GRN_API EntityTopdown *hitboxTopdown(EntityTopdown *entity, Blocks *blocks, int *ignoredType,
                                     int sizeIgnored);
/*recupere les blocks a partir dun fichier*/
GRN_API Blocks *getBlocksFromFile(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                                  int excludeElement);

/*Camera */
GRN_API void cameraUpdate(Camera *cam, float targetX, float targetY, float dt);

/* Enregistre un message de log avec le niveau spécifié */
GRN_API void logMessage(int level, char *message);

/* Change le niveau minimum de log affiché */
GRN_API void setLogLvl(int level);
/* Ajoute une image au tableau d'affichage */
GRN_API void draw(const char *path, float x, float y, Uint16 width, Uint16 height, bool flip,
                  Uint16 rotation, Uint8 alpha);

/* Ajoute du texte au tableau d'affichage avec police custom */
GRN_API void drawText(const char *font_path, const char *text, float x, float y, Uint16 scale,
                      bool flip, float spacing, Uint16 rotation, Uint8 alpha);

/* Charge toutes les images d'un dossier en mémoire */
GRN_API void loadImageFolder(const char *folder);
/* Libère toutes les images en mémoire */
GRN_API void freeImageFolder(void);

/* Définit l'icône de la fenêtre */
GRN_API void setIcon(const char *path);

/* Dessine un rectangle plein ou vide */
GRN_API void drawRect(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green,
                      Uint8 blue, Uint8 alpha, bool filled);

/* Dessine un cercle plein ou vide */
GRN_API void drawCircle(float x, float y, Sint16 radius, Uint8 red, Uint8 green, Uint8 blue,
                        bool filled);

/* Dessine un triangle plein ou vide */
GRN_API void drawTriangle(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green,
                          Uint8 blue, bool filled);

/* Dessine une ligne entre deux points */
GRN_API void drawLine(float x1, float y1, float x2, float y2, Uint8 red, Uint8 green, Uint8 blue);

/* Fonction de test pour les shaders */
GRN_API void test_shaders(void);

/* Lance la lecture d'un son avec options de boucle et canal */
GRN_API void playSound(const char *path, int loop, int channel, int volume);

/* Arrête la lecture d'un son identifié par son chemin */
GRN_API void stopSound(const char *path);

/* Met en pause la lecture d'un son identifié par son chemin */
GRN_API void pauseSound(const char *path);

/* Reprend la lecture d'un son en pause */
GRN_API void resumeSound(const char *path);

/* Arrête la lecture d'un canal audio spécifique */
GRN_API void stopChannel(int channel);

/* Met en pause la lecture d'un canal audio spécifique */
GRN_API void pauseChannel(int channel);

/* Reprend la lecture d'un canal audio en pause */
GRN_API void resumeChannel(int channel);

/* Charge tous les fichiers sonores d'un dossier en mémoire */
GRN_API void loadSongFolder(const char *folder);

/* Libère tous les sons en mémoire */
GRN_API void freeSongFolder(void);

/* Vérifie si une touche vient d'être pressée (front montant) */
GRN_API int keyJustPressed(const char *key_name);

/* Vérifie si une touche est maintenue enfoncée */
GRN_API int keyPressed(const char *key_name);

/* Affiche ou masque le curseur de la souris */
GRN_API void showCursor(bool visible);

/* Vérifie si un bouton de manette est maintenu enfoncé */
GRN_API int buttonPressed(const char *button_name, unsigned char index);

/* Vérifie si un bouton de manette vient d'être pressé (front montant) */
GRN_API int buttonJustPressed(const char *button_name, unsigned char index);

/* Initialise les manettes/joysticks à partir d'un index */
GRN_API void initController(unsigned char index);

/* Récupère les axes des joysticks avec zone morte (retourne 6 floats) */
GRN_API float *getJoysticks(float dead_zone, unsigned char index);

/* Ferme la manette */
GRN_API void closeController(unsigned char index);

/* Ferme le joystick */
GRN_API void closeJoystick(unsigned char index);

/* Ferme la manette et le joystick */
GRN_API void closeTheController(unsigned char index);

/* Récupère la position X de la souris */
GRN_API int mouseX(void);

/* Récupère la position Y de la souris */
GRN_API int mouseY(void);

/* Vérifie si le bouton gauche de la souris vient d'être pressé */
GRN_API int mouseLeftJustPressed(void);

/* Vérifie si le bouton droit de la souris vient d'être pressé */
GRN_API int mouseRightJustPressed(void);

/* Vérifie si le bouton gauche de la souris est maintenu enfoncé */
GRN_API int mouseLeftPressed(void);

/* Vérifie si le bouton droit de la souris est maintenu enfoncé */
GRN_API int mouseRightPressed(void);

/*retourne le scrool vertical de la souris 1 ou -1*/
GRN_API int mouseScrollVertical(void);

/*retourne le scrool vertical de la souris 1 ou -1*/
GRN_API int mouseScrollHorizontal(void);

/* Récupère le delta-time (temps écoulé depuis la dernière frame) */
GRN_API float delta(void);

/* Récupère les FPS actuels */
GRN_API float fps(void);

/* Récupère le nombre de frames écoulées */
GRN_API unsigned int frameCount(void);

/* Récupère le décalage X de la fenêtre */
GRN_API int offsetX(void);

/* Récupère le décalage Y de la fenêtre */
GRN_API int offsetY(void);

/* Récupère la largeur actuelle de la fenêtre */
GRN_API int currentWidth(void);

/* Récupère la hauteur actuelle de la fenêtre */
GRN_API int currentHeight(void);

/* Récupère la largeur de l'univers */
GRN_API int universeWidth(void);

/* Récupère la hauteur de l'univers */
GRN_API int universeHeight(void);

/*lancer le moteur lua*/
GRN_API void initializeLua(const char *filePath);

#ifdef __cplusplus
}
#endif

#endif /* GRNGAME_H */
