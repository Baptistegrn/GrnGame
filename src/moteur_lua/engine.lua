local ffi = require("ffi")

ffi.cdef [[
    /* typedefs sdl -> c */
    typedef uint8_t  Uint8;
    typedef uint16_t Uint16;
    typedef int16_t  Sint16;
    typedef int32_t  Sint32;
    typedef uint32_t Uint32;

    /* enum log level */
    typedef enum { GRN_LOG_DEBUG=0, GRN_LOG_INFO=1, GRN_LOG_WARNING=2, GRN_LOG_ERROR=3 } GLogLevel;

    /* structures */
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

    /* moteur */
    void initialize(int h, int w, float fps, int bars, const char *title, UpdateCallback cb);
    void stop(void);
    void windowedMinimised(int coeff);
    void windowed(int width, int height);
    void windowedFullscreen(void);
    void fullscreen(void);
    void clearScreen(int r, int g, int b);
    void logMessage(int level, const char *message);
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
    bool windowMinimised(void);

    /* blocks */
    Blocks *createBlocks();
    int getBlocksSize(Blocks *blocks);
    void addBlock(Blocks *blocks, Block *block);
    void freeBlocks(Blocks *blocks);
    Blocks *getBlocksFromFile(const char *path, Uint8 px, Uint8 py, char sep, int excl);

    /* physique,camera */
    EntityPlatformer *hitboxPlatformer(EntityPlatformer *ent, Blocks *blocs, float v_max, float corr, int *type_lock, int size);
    EntityTopdown *hitboxTopdown(EntityTopdown *ent, Blocks *blocs, int *type_lock, int size);
    void cameraUpdate(Camera *c, float tx, float ty, float dt);

    /* graphiques */
    void draw(const char *path, float x, float y, Uint16 w, Uint16 h, bool flip, Uint16 rotP, Uint16 rot, Uint8 a);
    void drawSprite(Sprite *sprite, Sint16 index, float x, float y, Sint16 w, Sint16 h, bool flip, Uint16 rotP, Uint16 rot, Uint8 alpha);
    Sprite *createSprite(const char *id, Sint16 width, Sint16 height);
    void drawText(const char *font, const char *txt, float x, float y, Uint16 sc, bool f, float sp, Uint16 rotP, Uint16 rot, Uint8 a);
    void loadImageFolder(const char *folder);
    void freeImageFolder(void);
    void setIcon(const char *path);
    void drawRect(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled);
    void drawCircle(float x, float y, Sint16 radius, Uint8 r, Uint8 g, Uint8 b,Uint8 a, bool filled);
    void drawTriangle(float x, float y, Sint16 w, Sint16 h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled);
    void drawLine(float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawParticles(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r, Uint8 *g, Uint8 *b, int size);

    /* audio */
    void playSound(const char *path, int loop, int channel, int volume);
    void stopSound(const char *path);
    void pauseSound(const char *path);
    void resumeSound(const char *path);
    void stopChannel(int channel);
    void pauseChannel(int channel);
    void resumeChannel(int channel);
    void loadSongFolder(const char *folder);
    void freeSongFolder(void);

    /* input */
    int keyJustPressed(const char *key);
    int keyPressed(const char *key);
    void showCursor(bool visible);
    int buttonPressed(const char *btn, unsigned char idx);
    int buttonJustPressed(const char *btn, unsigned char idx);
    void initController(unsigned char idx);
    float *getJoysticks(float dead, unsigned char idx);
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

-- Helper conversion tables Lua -> C
local function tableToIntArray(t)
    if not t or #t == 0 then return nil, 0 end
    local n = #t
    local arr = ffi.new("int[?]", n)
    for i = 1, n do arr[i - 1] = t[i] end
    return arr, n
end

-- Block
local Block = {}
Block.__index = Block
--classe
function Block.new(x, y, w, h, type)
    local b = ffi.new("Block")
    -- parametres par defaut
    b.x, b.y, b.w, b.h, b.type = x or 0, y or 0, w or 16, h or 16, type or 0
    return b
end

ffi.metatype("Block", Block)

-- blocks liste de blocks
local Blocks = {}
Blocks.__index = Blocks

function Blocks.new()
    local ptr = C.createBlocks()
    ffi.gc(ptr, C.freeBlocks) -- Garbage collection automatique
    return ptr
end

-- charger une carte de blocks depuis un fichier
function Blocks.fromFile(path, px, py, sep, excl)
    -- parametres par defaut
    local ptr = C.getBlocksFromFile(path, px or 16, py or 16, string.byte(sep or ","), excl or 0)
    if ptr ~= nil then
        ffi.gc(ptr, C.freeBlocks)
    end
    return ptr
end

-- ajouter block
function Blocks:add(block)
    C.addBlock(self, block)
end

-- taille
function Blocks:__len()
    return C.getBlocksSize(self)
end

-- itérateur pour les blocks
function Blocks:each()
    local i = 0
    local n = C.getBlocksSize(self)
    return function()
        if i < n then
            --accès au tableau de la structure C
            local b = self.tab[i]
            i = i + 1
            return i, b
        end
    end
end

ffi.metatype("Blocks", Blocks)

-- Entite
local EntityPlatformerMethods = {}
EntityPlatformerMethods.__index = EntityPlatformerMethods
function EntityPlatformerMethods:updatePhysics(blocks, max_fall_speed, wall_correction, type_lock)
    -- transformation table lua en tableau c
    local arr, taille = tableToIntArray(type_lock)

    C.hitboxPlatformer(self, blocks, max_fall_speed, wall_correction, arr, taille)
end

ffi.metatype("EntityPlatformer", EntityPlatformerMethods)
-- classe
function Engine.newEntityPlatformer(x, y, w, h, gravity, powerJump)
    local e = ffi.new("EntityPlatformer")
    -- parametres par defaut
    e.x, e.y, e.width, e.height = x or 0, y or 0, w or 16, h or 16
    e.gravity, e.powerJump = gravity or 400.0, powerJump or -200
    e.inSky, e.speedY, e.requestJump = true, 0, false
    e.leftLock, e.rightLock = false, false
    return e
end

local EntityTopdownMethods = {}
EntityTopdownMethods.__index = EntityTopdownMethods
-- update
function EntityTopdownMethods:updatePhysics(blocks, type_lock)
    local arr, taille = tableToIntArray(type_lock)
    C.hitboxTopdown(self, blocks, arr, taille)
end

ffi.metatype("EntityTopdown", EntityTopdownMethods)
--classe
function Engine.newEntityTopdown(x, y, w, h)
    local e = ffi.new("EntityTopdown")
    --parametres par defaut
    e.x, e.y, e.width, e.height = x or 0, y or 0, w or 16, h or 16
    return e
end

-- Camera
local CameraMethods = {}
CameraMethods.__index = CameraMethods
--update
function CameraMethods:update(tx, ty, dt) C.cameraUpdate(self, tx, ty, dt) end

ffi.metatype("Camera", CameraMethods)
--classe
function Engine.newCamera(w, h, smooth)
    local c = ffi.new("Camera")
    -- parametres par defaut
    c.w, c.h, c.smooth_factor = w or 160, h or 90, smooth or 0.1
    return c
end

-- Sprites
local SpriteMethods = {}
SpriteMethods.__index = SpriteMethods
function SpriteMethods:draw(index, x, y, w, h, flip, rot, alpha)
    -- parametres par defaut
    C.drawSprite(self, index or 0, x or 0, y or 0, w or self.taillex, h or self.tailley, flip or false, rot or 0,
        alpha or 255)
end

ffi.metatype("Sprite", SpriteMethods)

-- Engine
Engine.Block = Block
Engine.Blocks = Blocks

-- Core
Engine.initialize = C.initialize
Engine.stop = C.stop
Engine.windowedMinimised = C.windowedMinimised
Engine.windowed = C.windowed
Engine.windowedFullscreen = C.windowedFullscreen
Engine.fullscreen = C.fullscreen
Engine.clear = C.clearScreen
Engine.delta = C.delta
Engine.fps = C.fps
Engine.log = C.logMessage
Engine.setLogLvl = C.setLogLvl
Engine.minimised = C.windowMinimised
Engine.frameCount = C.frameCount


-- Graphics
Engine.draw = C.draw
Engine.createSprite = C.createSprite
Engine.drawRect = C.drawRect
Engine.drawCircle = C.drawCircle
Engine.drawTriangle = C.drawTriangle
Engine.drawLine = C.drawLine
Engine.drawText = C.drawText
Engine.loadImageFolder = C.loadImageFolder
Engine.freeImageFolder = C.freeImageFolder
Engine.setIcon = C.setIcon

-- Audio
Engine.playSound = C.playSound
Engine.stopSound = C.stopSound
Engine.pauseSound = C.pauseSound
Engine.resumeSound = C.resumeSound
Engine.loadSongFolder = C.loadSongFolder
Engine.freeSongFolder = C.freeSongFolder

-- Input
Engine.keyPressed = C.keyPressed
Engine.keyJustPressed = C.keyJustPressed
Engine.mouseX = C.mouseX
Engine.mouseY = C.mouseY
Engine.mouseLeftJustPressed = C.mouseLeftJustPressed
Engine.mouseRightJustPressed = C.mouseRightJustPressed

-- Window info
Engine.universeWidth = C.universeWidth
Engine.universeHeight = C.universeHeight
Engine.offsetX = C.offsetX
Engine.offsetY = C.offsetY
Engine.currentWidth = C.currentWidth
Engine.currentHeight = C.currentHeight
return Engine
