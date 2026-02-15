/*
 * GrnGame Engine C API
 * Provides all graphics, audio, games and input functionality for games.
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
GRN_API void initialize(int heightU, int widthU, float fps_target, bool black_bars,
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

/* Creates an entity for top-down physics */
GRN_API EntityTopdown *createEntityTopdown(float x, float y, float w, float h);

/* Creates an entity for platformer physics */
GRN_API EntityPlatformer *createEntityPlatformer(float x, float y, float w, float h,
                                                 float force_saut, float gravite);

/* Frees an entity used for top-down physics */
GRN_API void freeEntityTopdown(EntityTopdown *entite);

/* Frees an entity used for platformer physics */
GRN_API void freeEntityPlatformer(EntityPlatformer *entite);

/* Creates an empty blocks container */
GRN_API Blocks *createBlocks();

/* Returns the number of blocks in the container */
GRN_API int getBlocksSize(Blocks *blocks);

/* Adds a block to the container */
GRN_API void addBlock(Blocks *blocks, Block *block);

/* Frees all blocks in the container */
GRN_API void freeBlocks(Blocks *blocks);

/* Creates a single block with position, size and type */
GRN_API Block *createBlock(float x, float y, float w, float h, int type);

/* Frees a single block */
GRN_API void freeBlock(Block *block);

/* Updates camera position with smooth interpolation */
GRN_API void cameraUpdate(Camera *cam, float targetX, float targetY, float dt);

/* Creates a smooth-follow camera */
GRN_API Camera *createCamera(float x, float y, float smooth_factor, int w, int h);

/* Frees a camera */
GRN_API void freeCamera(Camera *cam);

/* Logs a message with the specified severity level */
GRN_API void logMessage(int level, const char *message);

/* Sets the minimum log level to display */
GRN_API void setLogLvl(int level);

/* Draws an image at the specified position */
GRN_API void draw(const char *path, float x, float y, Uint16 coeff, bool flip, Uint16 rotationP,
                  Uint16 rotation, Uint8 alpha);

/* Draws a sprite at the specified position */
GRN_API void drawSprite(Sprite *sprite, Sint16 index, float x, float y, Sint16 coeff, bool flip,
                        Uint16 rotation, Uint8 alpha);
/* Draws a particle array with individual transforms and colors */
GRN_API void drawParticles(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r, Uint8 *g,
                           Uint8 *b, int size);

/* Creates a sprite descriptor from an image ID */
GRN_API Sprite *createSprite(const char *id, Sint16 width, Sint16 height);

/* Frees a sprite */
GRN_API void freeSprite(Sprite *sprite);

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
GRN_API bool keyJustPressed(const char *key_name);

/* Returns true if a key is currently held down */
GRN_API bool keyPressed(const char *key_name);

/* Shows or hides the mouse cursor */
GRN_API void showCursor(bool visible);

/* Returns true if a controller button is currently held down */
GRN_API bool buttonPressed(const char *button_name, unsigned char index);

/* Returns true if a controller button was just pressed this frame */
GRN_API bool buttonJustPressed(const char *button_name, unsigned char index);

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
GRN_API bool mouseLeftJustPressed(void);

/* Returns true if the right mouse button was just pressed */
GRN_API bool mouseRightJustPressed(void);

/* Returns true if the left mouse button is currently held down */
GRN_API bool mouseLeftPressed(void);

/* Returns true if the right mouse button is currently held down */
GRN_API bool mouseRightPressed(void);

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

/* put key for image */
GRN_API void setImageKey(int index, int value);

/* put iv for image */
GRN_API void setImageIv(int index, int value);

/* put key for song */
GRN_API void setSongKey(int index, int value);

/* put iv for song */
GRN_API void setSongIv(int index, int value);

/* load a file on the disk */
GRN_API void loadJsonFile(const char *filename);

/* save a file on the disk */
GRN_API void saveJsonFile(const char *filename);

/* delete a file from the disk and memory */
GRN_API void deleteJsonFile(const char *filename);

/* write an int in a file */
GRN_API void writeIntToJson(const char *filename, const char *varName, int value);

/* write a double in a file */
GRN_API void writeDoubleToJson(const char *filename, const char *varName, double value);

/* write a string in a file */
GRN_API void writeStringToJson(const char *filename, const char *varName, const char *value);

/* read an int from a file */
GRN_API int readIntFromJson(const char *filename, const char *varName);

/* read a double from a file */
GRN_API double readDoubleFromJson(const char *filename, const char *varName);

/* read a string from a file */
GRN_API char *readStringFromJson(const char *filename, const char *varName);

/* delete a variable from a file */
GRN_API void deleteVariableFromJson(const char *filename, const char *varName);

/* put a value in the encryption key */
GRN_API void setJsonEncryptionKey(int index, int value);

/* put a value in the initialization vector */
GRN_API void setJsonEncryptionIV(int index, int value);

/* get the type of variable 0=UNKNOW, 1=NUMBER, 2=STRING */
GRN_API int getJsonVariableType(const char *filename, const char *varName);

/* try if a file exist or not */
GRN_API bool FileExists(const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* GRNGAME_H */
