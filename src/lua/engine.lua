local ffi = require("ffi")

ffi.cdef [[
    typedef uint8_t  Uint8;
    typedef uint16_t Uint16;
    typedef int16_t  Sint16;
    typedef int32_t  Sint32;
    typedef uint32_t Uint32;

    typedef enum { GRN_LOG_DEBUG=0, GRN_LOG_INFO=1, GRN_LOG_WARNING=2, GRN_LOG_ERROR=3 } GLogLevel;

    typedef struct {
        const char *id;
        Sint16 taillex, tailley;
    } Sprite;

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

    void initialize(int h, int w, float fps, int bars, const char *title, UpdateCallback cb);
    void stop(void);
    void resize(int w, int h, bool fullscreen_requested,bool window_fullscreen_requested);
    void clearScreen(int r, int g, int b);
    void logMessage(int level, char *message);
    void setLogLvl(int level);
    float delta(void);
    float fps(void);
    unsigned int frameCount(void);
    int offsetX(void);
    int offsetY(void);
    int currentWidth(void);
    int currentHeight(void);
    int universeWidth(void);
    int universeHeight(void);

    EntityPlatformer *hitboxPlatformer(EntityPlatformer *ent, Blocks *blocs, float v_max, float corr, int *type_lock, int size);
    EntityTopdown *hitboxTopdown(EntityTopdown *ent, Blocks *blocs, int *type_lock, int size);
    Blocks *getBlocksFromFile(const char *path, Uint8 px, Uint8 py, char sep, int excl);

    void cameraUpdate(Camera *c, float tx, float ty, float dt);

    void draw(const char *path, float x, float y, Uint16 w, Uint16 h, bool flip, Uint16 rot, Uint8 a);
    void drawSprite(Sprite *sprite, Sint16 index, float x, float y, Sint16 w, Sint16 h, bool flip, Uint16 rotation, Uint8 alpha);
    Sprite *createSprite(const char *id, Sint16 width, Sint16 height);
    void drawText(const char *font, const char *txt, float x, float y, Uint16 sc, bool f, float sp, Uint16 r, Uint8 a);
    void loadImageFolder(const char *folder);
    void freeImageFolder(void);
    void setIcon(const char *path);

    void drawRect(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled);
    void drawCircle(float x, float y, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, bool filled);
    void drawTriangle(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b, bool filled);
    void drawLine(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b);

    void drawParticles(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r, Uint8 *g, Uint8 *b, int size);

    void playSound(const char *path, int loop, int channel, int volume);
    void stopSound(const char *path);
    void pauseSound(const char *path);
    void resumeSound(const char *path);
    void stopChannel(int channel);
    void pauseChannel(int channel);
    void resumeChannel(int channel);
    void loadSongFolder(const char *folder);
    void freeSongFolder(void);

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
    int mouseScrollVertical(void);
    int mouseScrollHorizontal(void);
]]

local C = ffi.C
local Engine = {}

local function tableToIntArray(t)
    if not t or #t == 0 then return nil, 0 end
    local n = #t
    local arr = ffi.new("int[?]", n)
    for i = 1, n do arr[i - 1] = t[i] end
    return arr, n
end

local SpriteMethods = {}
SpriteMethods.__index = SpriteMethods
function SpriteMethods:draw(index, x, y, w, h, flip, rotation, alpha)
    C.drawSprite(self, index, x, y, w, h, flip or false, rotation or 0, alpha or 255)
end

ffi.metatype("Sprite", SpriteMethods)

local Block = {}
Block.__index = Block
function Block.new(x, y, w, h, type) return ffi.new("Block", { x, y, w, h, type }) end

ffi.metatype("Block", Block)

local Blocks = {}
Blocks.__index = Blocks
function Blocks:__len() return self.size end

function Blocks.new(path, grid_sizex, grid_sizey, separated, exclusion)
    return C.getBlocksFromFile(path, grid_sizex, grid_sizey, string.byte(separated), exclusion)
end

ffi.metatype("Blocks", Blocks)

local EntityPlatformerMethods = {}
EntityPlatformerMethods.__index = EntityPlatformerMethods
function EntityPlatformerMethods:updatePhysics(blocks, max_fall_speed, wall_correction, type_lock)
    local arr, len = tableToIntArray(type_lock)
    C.hitboxPlatformer(self, blocks, max_fall_speed, wall_correction, arr, len)
end

ffi.metatype("EntityPlatformer", EntityPlatformerMethods)

function Engine.newEntityPlatformer(x, y, w, h, gravity, powerJump)
    local e = ffi.new("EntityPlatformer")
    e.x, e.y, e.width, e.height = x or 0, y or 0, w, h
    e.gravity, e.powerJump = gravity or 400.0, powerJump or -200
    e.inSky, e.speedY, e.requestJump = true, 0, false
    e.leftLock, e.rightLock = false, false
    return e
end

local EntityTopdownMethods = {}
EntityTopdownMethods.__index = EntityTopdownMethods
function EntityTopdownMethods:updatePhysics(blocks, type_lock)
    local arr, len = tableToIntArray(type_lock)
    C.hitboxTopdown(self, blocks, arr, len)
end

ffi.metatype("EntityTopdown", EntityTopdownMethods)

function Engine.newEntityTopdown(x, y, w, h)
    local e = ffi.new("EntityTopdown")
    e.x, e.y, e.width, e.height = x, y, w, h
    return e
end

local CameraMethods = {}
CameraMethods.__index = CameraMethods
function CameraMethods:update(target_x, target_y, dt) C.cameraUpdate(self, target_x, target_y, dt) end

ffi.metatype("Camera", CameraMethods)

function Engine.newCamera(w, h, smooth_factor)
    local c = ffi.new("Camera")
    c.w, c.h, c.smooth_factor = w, h, smooth_factor
    return c
end

--convertir des tables Lua en tableaux FFI
--lourd
function Engine.drawParticles(posx, posy, rotation, a, r, g, b)
    local taille = #posx
    -- Créer les tableaux FFI
    local x_arr = ffi.new("float[?]", taille)
    local y_arr = ffi.new("float[?]", taille)
    local rot_arr = ffi.new("Uint16[?]", taille)
    local a_arr = ffi.new("Uint8[?]", taille)
    local r_arr = ffi.new("Uint8[?]", taille)
    local g_arr = ffi.new("Uint8[?]", taille)
    local b_arr = ffi.new("Uint8[?]", taille)

    -- Remplir les tableaux
    for i = 1, taille do
        x_arr[i - 1] = posx[i]
        y_arr[i - 1] = posy[i]
        rot_arr[i - 1] = rotation[i] or 0
        a_arr[i - 1] = a[i] or 255
        r_arr[i - 1] = r[i]
        g_arr[i - 1] = g[i]
        b_arr[i - 1] = b[i]
    end

    -- Appeler la fonction C
    C.drawParticles(x_arr, y_arr, rot_arr, a_arr, r_arr, g_arr, b_arr, taille)
end

Engine.Block = Block
Engine.Blocks = Blocks
Engine.initialize = C.initialize
Engine.stop = C.stop
Engine.resize = C.resize
Engine.clear = C.clearScreen
Engine.draw = C.draw
Engine.createSprite = C.createSprite
Engine.drawSprite = C.drawSprite
Engine.drawRect = C.drawRect
Engine.drawText = C.drawText
Engine.drawCircle = C.drawCircle
Engine.drawTriangle = C.drawTriangle
Engine.drawLine = C.drawLine
Engine.loadImageFolder = C.loadImageFolder
Engine.freeImageFolder = C.freeImageFolder
Engine.setIcon = C.setIcon
Engine.playSound = C.playSound
Engine.stopSound = C.stopSound
Engine.pauseSound = C.pauseSound
Engine.resumeSound = C.resumeSound
Engine.stopChannel = C.stopChannel
Engine.pauseChannel = C.pauseChannel
Engine.resumeChannel = C.resumeChannel
Engine.loadSongFolder = C.loadSongFolder
Engine.freeSongFolder = C.freeSongFolder
Engine.delta = C.delta
Engine.fps = C.fps
Engine.log = C.logMessage
Engine.setLogLvl = C.setLogLvl
Engine.offsetX, Engine.offsetY = C.offsetX, C.offsetY
Engine.currentWidth, Engine.currentHeight = C.currentWidth, C.currentHeight
Engine.universeWidth, Engine.universeHeight = C.universeWidth, C.universeHeight
Engine.keyPressed = C.keyPressed
Engine.keyJustPressed = C.keyJustPressed
Engine.showCursor = C.showCursor
Engine.mouseX, Engine.mouseY = C.mouseX, C.mouseY
Engine.mouseLeftJustPressed = C.mouseLeftJustPressed
Engine.mouseRightJustPressed = C.mouseRightJustPressed
Engine.mouseLeftPressed, Engine.mouseRightPressed = C.mouseLeftPressed, C.mouseRightPressed
Engine.mouseScrollVertical = C.mouseScrollVertical
Engine.mouseScrollHorizontal = C.mouseScrollHorizontal
Engine.buttonPressed = C.buttonPressed
Engine.buttonJustPressed = C.buttonJustPressed
Engine.initController = C.initController
Engine.getJoysticks = C.getJoysticks
Engine.closeController = C.closeController
Engine.closeJoystick = C.closeJoystick
Engine.closeTheController = C.closeTheController

return Engine
