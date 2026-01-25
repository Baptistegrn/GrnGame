local ffi = require("ffi")

ffi.cdef [[
    // utilisation des types de SDL
    typedef uint8_t  Uint8;
    typedef uint16_t Uint16;
    typedef int16_t  Sint16;
    typedef int32_t  Sint32;
    typedef uint32_t Uint32;

    typedef enum { GRN_LOG_DEBUG=0, GRN_LOG_INFO=1, GRN_LOG_WARNING=2, GRN_LOG_ERROR=3 } GLogLevel;

    typedef struct {
        float x, y, w, h;
        int type;
    } Block;

    typedef struct {
        Block *tab;
        int size;
        int capacity;
    } Blocks;

    typedef struct {
        float x, y, width, height;
    } EntityTopdown;

    typedef struct {
        float x, y, speedY;
        float width, height;
        float gravity, powerJump;
        bool inSky, requestJump;
        bool leftLock,rightLock;
    } EntityPlatformer;

    typedef struct {
        float x, y, smooth_factor;
        int w, h;
    } Camera;

    typedef void (*UpdateCallback)(void);

    //utils
    void initialize(int h, int w, float fps, int bars, const char *title, const char *log, UpdateCallback cb);
    void stop(void);
    void resize(int w, int h, bool fullscreen_requested);
    void clearScreen(int r, int g, int b);
    void logMessage(int level, char *message);
    void setLogLvl(int level);
    //const
    float delta(void);
    float fps(void);
    unsigned int frameCount(void);
    int offsetX(void);
    int offsetY(void);

    // Physique
    EntityPlatformer *hitboxPlatformer(EntityPlatformer *ent, Blocks *blocs, float v_max, float corr, int *type_lock, int size);
    EntityTopdown *hitboxTopdown(EntityTopdown *ent, Blocks *blocs, int *type_lock, int size);
    Blocks *getBlocksFromFile(const char *path, Uint8 px, Uint8 py, char sep, int excl);

    // Camera
    void cameraUpdate(Camera *c, float tx, float ty, float dt);

    // Dessin
    void draw(const char *path, float x, float y, Uint16 w, Uint16 h, bool flip, Uint16 rot, Uint8 a);
    void drawText(const char *font, const char *txt, float x, float y, Uint16 sc, bool f, float sp, Uint16 r, Uint8 a);
    void loadImageFolder(const char *folder);
    void freeImageFolder(void);
    void setIcon(const char *path);

    // Primitives
    void drawRect(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled);
    void drawCircle(float x, float y, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, bool filled);
    void drawTriangle(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b, bool filled);
    void drawLine(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b);

    // Audio
    void playSound(const char *path, int loop, int channel, int volume);
    void stopSound(const char *path);
    void pauseSound(const char *path);
    void resumeSound(const char *path);
    void stopChannel(int channel);
    void pauseChannel(int channel);
    void resumeChannel(int channel);
    void loadSongFolder(const char *folder);
    void freeSongFolder(void);

    // Inputs
    int keyJustPressed(const char *key);
    int keyPressed(const char *key);
    void showCursor(bool visible);
    int buttonPressed(const char *btn, unsigned char idx);
    int buttonJustPressed(const char *btn, unsigned char idx);
    void initController(unsigned char idx);
    float *getJoysticks(float dead, unsigned char idx);
    void closeController(unsigned char idx);
    void closeJoystick(unsigned char idx);
    void closeTheController(unsigned char idx);

    int mouseX(void);
    int mouseY(void);
    int mouseLeftJustPressed(void);
    int mouseRightJustPressed(void);
    int mouseLeftPressed(void);
    int mouseRightPressed(void);
]]

local C = ffi.C
local Engine = {}

-- Convertit une table Lua en int* pour type_lock
local function tableToIntArray(t)
    if not t or #t == 0 then
        return nil, 0
    end

    local n = #t
    local arr = ffi.new("int[?]", n)

    for i = 1, n do
        arr[i - 1] = t[i]
    end

    return arr, n
end

--class block
local Block = {}
Block.__index = Block

function Block.new(x, y, w, h, type)
    return ffi.new("Block", { x, y, w, h, type })
end

ffi.metatype("Block", Block)

--class blocks
local Blocks = {}
Blocks.__index = Blocks

function Blocks:__len()
    return self.size
end

function Blocks.new(path, grid_sizex, grid_sizey, separated, exclusion)
    return C.getBlocksFromFile(path, grid_sizex, grid_sizey, string.byte(separated), exclusion)
end

ffi.metatype("Blocks", Blocks)

--class entite platformer
local EntityPlatformerMethods = {}
EntityPlatformerMethods.__index = EntityPlatformerMethods

function EntityPlatformerMethods:updatePhysics(blocks, max_fall_speed, wall_correction, type_lock)
    local arr, len = tableToIntArray(type_lock)
    C.hitboxPlatformer(self, blocks, max_fall_speed, wall_correction, arr, len)
end

ffi.metatype("EntityPlatformer", EntityPlatformerMethods)

function Engine.newEntityPlatformer(x, y, w, h, gravity, powerJump)
    local e = ffi.new("EntityPlatformer")
    e.x = x or 0
    e.y = y or 0
    e.width = w
    e.height = h
    e.gravity = gravity or 400.0
    e.powerJump = powerJump or -200
    e.inSky = true
    e.speedY = 0
    e.requestJump = false
    e.leftLock = false;
    e.rightLock = false;
    return e
end

--class entite topdown
local EntityTopdownMethods = {}
EntityTopdownMethods.__index = EntityTopdownMethods

function EntityTopdownMethods:updatePhysics(blocks, type_lock)
    local arr, len = tableToIntArray(type_lock)
    C.hitboxTopdown(self, blocks, arr, len)
end

ffi.metatype("EntityTopdown", EntityTopdownMethods)

function Engine.newEntityTopdown(x, y, w, h)
    local e = ffi.new("EntityTopdown")
    e.x = x;
    e.y = y;
    e.width = w;
    e.height = h;
    return e
end

--class camera
local CameraMethods = {}
CameraMethods.__index = CameraMethods

function CameraMethods:update(target_x, target_y, dt)
    C.cameraUpdate(self, target_x, target_y, dt)
end

ffi.metatype("Camera", CameraMethods)

function Engine.newCamera(w, h, smooth_factor)
    local c = ffi.new("Camera")
    c.w = w;
    c.h = h;
    c.smooth_factor = smooth_factor;
    return c
end

--export
Engine.Block = Block
Engine.Blocks = Blocks

-- Core
Engine.initialize = C.initialize
Engine.stop = C.stop
Engine.resize = C.resize
Engine.clear = C.clearScreen

-- Render
Engine.draw = C.draw
Engine.drawRect = C.drawRect
Engine.drawText = C.drawText
Engine.drawCircle = C.drawCircle
Engine.drawTriangle = C.drawTriangle
Engine.drawLine = C.drawLine
Engine.loadImageFolder = C.loadImageFolder
Engine.freeImageFolder = C.freeImageFolder
Engine.setIcon = C.setIcon

-- Audio
Engine.playSound = C.playSound
Engine.stopSound = C.stopSound
Engine.pauseSound = C.pauseSound
Engine.resumeSound = C.resumeSound
Engine.stopChannel = C.stopChannel
Engine.pauseChannel = C.pauseChannel
Engine.resumeChannel = C.resumeChannel
Engine.loadSongFolder = C.loadSongFolder
Engine.freeSongFolder = C.freeSongFolder

-- Utils
Engine.delta = C.delta
Engine.fps = C.fps
Engine.log = C.logMessage
Engine.setLogLvl = C.setLogLvl
Engine.offsetX = C.offsetX
Engine.offsetY = C.offsetY

-- Input Clavier/Souris
Engine.keyPressed = C.keyPressed
Engine.keyJustPressed = C.keyJustPressed
Engine.showCursor = C.showCursor
Engine.mouseX = C.mouseX
Engine.mouseY = C.mouseY
Engine.mouseLeftJustPressed = C.mouseLeftJustPressed
Engine.mouseRightJustPressed = C.mouseRightJustPressed
Engine.mouseLeftPressed = C.mouseLeftPressed
Engine.mouseRightPressed = C.mouseRightPressed

-- Input Manette
Engine.buttonPressed = C.buttonPressed
Engine.buttonJustPressed = C.buttonJustPressed
Engine.initController = C.initController
Engine.getJoysticks = C.getJoysticks
Engine.closeController = C.closeController
Engine.closeJoystick = C.closeJoystick
Engine.closeTheController = C.closeTheController

return Engine
