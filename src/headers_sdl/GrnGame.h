/*
 * GrnGame Engine C API
 * Exposes all graphics, audio and input features
 */

#ifndef GRNGAME_H
#define GRNGAME_H
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

typedef enum {
    GRN_LOG_DEBUG = 0,
    GRN_LOG_INFO = 1,
    GRN_LOG_WARNING = 2,
    GRN_LOG_ERROR = 3
} GLogLevel;

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

/* Initialize the game window */
GRN_API void initialize(int height, int width, float fps_target, int scale_coefficient,
                        int black_bars, const char *window_title, const char *log_path,
                        UpdateCallback update_func);

/* Stop the engine and close the application */
GRN_API void stop(void);

/* Toggle between fullscreen and windowed mode */
GRN_API void toggleFullscreen(void);

/* Clear the screen with the specified color */
GRN_API void clearScreen(int red, int green, int blue);

/* Calculate 2D platformer physics with collision handling */
GRN_API EntityPlatformer *hitboxPlatformer(EntityPlatformer *entite, Blocks *blocs,
                                           float vitesse_max_chute, float correction_mur,
                                           int *ignored_type, int size);

GRN_API EntityTopdown *hitboxTopdown(EntityTopdown *entity, Blocks *blocks, int *types_ignorer,
                                     int nb_ignorer);

/* Load blocks from a file */
GRN_API Blocks *getBlocksFromFile(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                                  int excludeElement);

/* Update camera position smoothly */
GRN_API void cameraUpdate(Camera *cam, float target_x, float target_y, float dt);

/* Log a message with the specified level */
GRN_API void logMessage(int level, char *message);

/* Change the minimum log level displayed */
GRN_API void setLogLvl(int level);

/* Add an image to the display list */
GRN_API void draw(const char *path, float x, float y, Uint16 width, Uint16 height, bool flip,
                  Uint16 rotation, Uint8 alpha);

/* Add text to the display list with custom font */
GRN_API void drawText(const char *font_path, const char *text, float x, float y, Uint16 scale,
                      bool flip, float spacing, Uint16 rotation, Uint8 alpha);

/* Load all images from a folder into memory */
GRN_API void loadImageFolder(const char *folder);

/* Free all loaded images from memory */
GRN_API void freeImageFolder(void);

/* Set the window icon */
GRN_API void setIcon(const char *path);

/* Draw a filled or outlined rectangle */
GRN_API void drawRect(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green,
                      Uint8 blue, Uint8 alpha, bool filled);

/* Draw a filled or outlined circle */
GRN_API void drawCircle(float x, float y, Sint16 radius, Uint8 red, Uint8 green, Uint8 blue,
                        bool filled);

/* Draw a filled or outlined triangle */
GRN_API void drawTriangle(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green,
                          Uint8 blue, bool filled);

/* Draw a line between two points */
GRN_API void drawLine(float x1, float y1, float x2, float y2, Uint8 red, Uint8 green, Uint8 blue);

/* Test function for shaders */
GRN_API void test_shaders(void);

/* Play a sound with loop and channel options */
GRN_API void playSound(const char *path, int loop, int channel, int volume);

/* Stop playback of a sound identified by its path */
GRN_API void stopSound(const char *path);

/* Pause playback of a sound identified by its path */
GRN_API void pauseSound(const char *path);

/* Resume playback of a paused sound */
GRN_API void resumeSound(const char *path);

/* Stop playback of a specific audio channel */
GRN_API void stopChannel(int channel);

/* Pause playback of a specific audio channel */
GRN_API void pauseChannel(int channel);

/* Resume playback of a paused audio channel */
GRN_API void resumeChannel(int channel);

/* Load all sound files from a folder into memory */
GRN_API void loadSongFolder(const char *folder);

/* Free all loaded sounds from memory */
GRN_API void freeSongFolder(void);

/* Check if a key was just pressed (rising edge) */
GRN_API int keyJustPressed(const char *key_name);

/* Check if a key is being held down */
GRN_API int keyPressed(const char *key_name);

/* Show or hide the mouse cursor */
GRN_API void showCursor(bool visible);

/* Check if a controller button is being held down */
GRN_API int buttonPressed(const char *button_name, unsigned char index);

/* Check if a controller button was just pressed (rising edge) */
GRN_API int buttonJustPressed(const char *button_name, unsigned char index);

/* Initialize controllers/joysticks from an index */
GRN_API void initController(unsigned char index);

/* Get joystick axes with dead zone (returns 6 floats) */
GRN_API float *getJoysticks(float dead_zone, unsigned char index);

/* Close the controller */
GRN_API void closeController(unsigned char index);

/* Close the joystick */
GRN_API void closeJoystick(unsigned char index);

/* Close both controller and joystick */
GRN_API void closeTheController(unsigned char index);

/* Get mouse X position */
GRN_API int mouseX(void);

/* Get mouse Y position */
GRN_API int mouseY(void);

/* Check if left mouse button was just pressed */
GRN_API int mouseLeftJustPressed(void);

/* Check if right mouse button was just pressed */
GRN_API int mouseRightJustPressed(void);

/* Check if left mouse button is being held down */
GRN_API int mouseLeftPressed(void);

/* Check if right mouse button is being held down */
GRN_API int mouseRightPressed(void);

/* Get delta-time (time elapsed since last frame) */
GRN_API float delta(void);

/* Get current FPS */
GRN_API float fps(void);

/* Get the elapsed frame count */
GRN_API unsigned int frameCount(void);

/* Get window X offset */
GRN_API int offsetX(void);

/* Get window Y offset */
GRN_API int offsetY(void);

/* Initialize the Lua engine */
GRN_API void initialisation_lua();

#ifdef __cplusplus
}
#endif

#endif /* GRNGAME_H */
