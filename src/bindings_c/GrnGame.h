/*
 * GrnGame Engine C API
 * Provides all graphics, audio and input functionality for games.
 */

#ifndef GRNGAME_H
#define GRNGAME_H

#include <SDL_stdinc.h>
#include <stdbool.h>

/* Platform-specific DLL export macros */
#if defined(_WIN32)
#if defined(GAME_EXPORTS)
#define GRN_API __declspec(dllexport)
#else
#define GRN_API __declspec(dllimport)
#endif
#else
#define GRN_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Log severity levels */
typedef enum { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 } LogLevel;

/* Sprite descriptor with texture ID and dimensions */
typedef struct {
    const char *id;
    Sint16 taillex, tailley;
} Sprite;

/* Single collision block with position, size and type */
typedef struct {
    float x;
    float y;
    float w;
    float h;
    int type;
} Block;

/* Dynamic array of collision blocks */
typedef struct {
    Block *tab;
    int size;
    int capacity;
} Blocks;

/* Entity for top-down physics */
typedef struct {
    float x;
    float y;
    float width;
    float height;
} EntityTopdown;

/* Entity for platformer physics with gravity and jump */
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

/* Smooth-follow camera structure */
typedef struct {
    float x;
    float y;
    float smooth_factor;
    int width;
    int height;
} Camera;

/* Callback function type for the update loop */
typedef void (*UpdateCallback)(void);

/* Initializes the engine with window size, FPS and starts the game loop */
GRN_API void initialize(int heightU, int widthU, float fps_target, int black_bars,
                        const char *window_title, UpdateCallback update_func);

/* Stops the engine and closes the application */
GRN_API void stop(void);

/* Switches to fullscreen mode */
GRN_API void fullscreen(void);

/* Switches to windowed maximized mode */
GRN_API void windowedFullscreen(void);

/* Switches to windowed mode with specific size */
GRN_API void windowed(int width, int height);

/* Switches to windowed mode with universe scaling factor */
GRN_API void windowedMinimised(int coeff);

/* Clears the screen with the specified color */
GRN_API void clearScreen(int red, int green, int blue);

/* Computes 2D platformer physics with collision detection */
GRN_API EntityPlatformer *hitboxPlatformer(EntityPlatformer *entite, Blocks *blocs,
                                           float vitesse_max_chute, float correction_mur,
                                           int *ignored_type, int size);

/* Computes top-down physics with collision detection */
GRN_API EntityTopdown *hitboxTopdown(EntityTopdown *entity, Blocks *blocks, int *ignoredType,
                                     int sizeIgnored);

/* Loads blocks from a file with grid spacing and separator */
GRN_API Blocks *getBlocksFromFile(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                                  int excludeElement);

/* Creates an empty blocks container */
GRN_API Blocks *createBlocks();

/* Returns the number of blocks in the container */
GRN_API int getBlocksSize(Blocks *blocks);

/* Adds a block to the container */
GRN_API void addBlock(Blocks *blocks, Block *block);

/* Frees all blocks in the container */
GRN_API void freeBlocks(Blocks *blocks);

/* Updates camera position with smooth interpolation */
GRN_API void cameraUpdate(Camera *cam, float targetX, float targetY, float dt);

/* Logs a message with the specified severity level */
GRN_API void logMessage(int level, const char *message);

/* Sets the minimum log level to display */
GRN_API void setLogLvl(int level);

/* Draws an image at the specified position */
GRN_API void draw(const char *path, float x, float y, Uint16 width, Uint16 height, bool flip,
                  Uint16 rotationP, Uint16 rotation, Uint8 alpha);

/* Draws a sprite at the specified position */
GRN_API void drawSprite(Sprite *sprite, float x, float y, Sint16 w, Sint16 u, bool flip,
                        Uint16 rotationP, Uint16 rotation, Uint8 alpha);
/* Draws a particle array with individual transforms and colors */
GRN_API void drawParticles(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r, Uint8 *g,
                           Uint8 *b, int size);

/* Creates a sprite descriptor from an image ID */
GRN_API Sprite *createSprite(const char *id, Sint16 width, Sint16 height);

/* Draws text using a custom bitmap font */
GRN_API void drawText(const char *font_path, const char *text, float x, float y, Uint16 scale,
                      bool flip, float spacing, Uint16 rotationP, Uint16 rotation, Uint8 alpha);

/* Loads all images from a folder into memory */
GRN_API void loadImageFolder(const char *folder);

/* Frees all loaded images from memory */
GRN_API void freeImageFolder(void);

/* Sets the window icon */
GRN_API void setIcon(const char *path);

/* Draws a filled or outlined rectangle */
GRN_API void drawRect(float x, float y, Sint16 width, Sint16 height, Uint8 red, Uint8 green,
                      Uint8 blue, Uint8 alpha, bool filled);

/* Draws a filled or outlined circle */
GRN_API void drawCircle(float x, float y, Sint16 radius, Uint8 red, Uint8 green, Uint8 blue,
                        Uint8 alpha, bool filled);

/* Draws a line between two points */
GRN_API void drawLine(float x1, float y1, float x2, float y2, Uint8 red, Uint8 green, Uint8 blue,
                      Uint8 alpha);

/* Shader test function */
GRN_API void test_shaders(void);

/* Plays a sound with loop, channel and volume options */
GRN_API void playSound(const char *path, int loop, int channel, int volume);

/* Stops a sound identified by its path */
GRN_API void stopSound(const char *path);

/* Pauses a sound identified by its path */
GRN_API void pauseSound(const char *path);

/* Resumes a paused sound */
GRN_API void resumeSound(const char *path);

/* Stops playback on a specific audio channel */
GRN_API void stopChannel(int channel);

/* Pauses playback on a specific audio channel */
GRN_API void pauseChannel(int channel);

/* Resumes playback on a paused audio channel */
GRN_API void resumeChannel(int channel);

/* Loads all sound files from a folder into memory */
GRN_API void loadSongFolder(const char *folder);

/* Frees all loaded sounds from memory */
GRN_API void freeSongFolder(void);

/* Returns true if a key was just pressed this frame */
GRN_API int keyJustPressed(const char *key_name);

/* Returns true if a key is currently held down */
GRN_API int keyPressed(const char *key_name);

/* Shows or hides the mouse cursor */
GRN_API void showCursor(bool visible);

/* Returns true if a controller button is currently held down */
GRN_API int buttonPressed(const char *button_name, unsigned char index);

/* Returns true if a controller button was just pressed this frame */
GRN_API int buttonJustPressed(const char *button_name, unsigned char index);

/* Initializes a game controller at the specified index */
GRN_API void initController(unsigned char index);

/* Returns joystick axes values with deadzone applied (6 floats) */
GRN_API float *getJoysticks(float dead_zone, unsigned char index);

/* Closes a game controller at the specified index */
GRN_API void closeController(unsigned char index);

/* Closes a joystick at the specified index */
GRN_API void closeJoystick(unsigned char index);

/* Closes both controller and joystick at the specified index */
GRN_API void closeTheController(unsigned char index);

/* Returns the mouse X position in game coordinates */
GRN_API int mouseX(void);

/* Returns the mouse Y position in game coordinates */
GRN_API int mouseY(void);

/* Returns true if the left mouse button was just pressed */
GRN_API int mouseLeftJustPressed(void);

/* Returns true if the right mouse button was just pressed */
GRN_API int mouseRightJustPressed(void);

/* Returns true if the left mouse button is currently held down */
GRN_API int mouseLeftPressed(void);

/* Returns true if the right mouse button is currently held down */
GRN_API int mouseRightPressed(void);

/* Returns vertical scroll delta (1 or -1) */
GRN_API int mouseScrollVertical(void);

/* Returns horizontal scroll delta (1 or -1) */
GRN_API int mouseScrollHorizontal(void);

/* Returns the delta time since last frame in seconds */
GRN_API float delta(void);

/* Returns the current frames per second */
GRN_API float fps(void);

/* Returns the total number of elapsed frames */
GRN_API unsigned int frameCount(void);

/* Returns the X offset of the game viewport */
GRN_API int offsetX(void);

/* Returns the Y offset of the game viewport */
GRN_API int offsetY(void);

/* Returns the current window width */
GRN_API int currentWidth(void);

/* Returns the current window height */
GRN_API int currentHeight(void);

/* Returns the game universe width */
GRN_API int universeWidth(void);

/* Returns the game universe height */
GRN_API int universeHeight(void);

/* Returns true if the window is minimised */
GRN_API bool windowMinimised(void);

/* Initializes and runs the Lua engine with the specified script */
GRN_API void initializeLua(const char *filePath);

#ifdef __cplusplus
}
#endif

#endif /* GRNGAME_H */
