---@meta
-- Fichier de definitions pour l'autocompletion de l'API GrnGame.
-- Ne pas executer, sert uniquement a l'editeur (VS Code, IntelliJ, etc.).

--
-- utils : fonctions utilitaires du moteur
--

---@class utils
utils = {}

--- Initialise le moteur de jeu
---@param height? integer hauteur de l'univers (defaut: 320)
---@param width? integer largeur de l'univers (defaut: 180)
---@param fps? number images par seconde (defaut: 60.0)
---@param black_bars? boolean bandes noires (defaut: false)
---@param window_title? string titre de la fenetre (defaut: "My Game")
---@param update_func? function fonction de mise a jour appelee chaque frame
function utils.initialize(height, width, fps, black_bars, window_title, update_func) end

--- Affiche un message dans les logs
---@param level integer niveau de log (0=debug, 1=info, 2=warning, 3=error)
---@param message string message a afficher
function utils.logMessage(level, message) end

--- Arrete le moteur
function utils.stop() end

--- Passe en plein ecran
function utils.fullscreen() end

--- Passe en plein ecran fenetre
function utils.windowedFullscreen() end

--- Passe en mode fenetre avec taille donnee
---@param width integer largeur
---@param height integer hauteur
function utils.windowed(width, height) end

--- Passe en mode fenetre minimisee avec coefficient
---@param coeff integer coefficient de redimensionnement
function utils.windowedMinimised(coeff) end

--- Efface l'ecran avec une couleur
---@param r integer rouge (0-255)
---@param g integer vert (0-255)
---@param b integer bleu (0-255)
function utils.cls(r, g, b) end

--- Change le niveau de log
---@param level integer niveau de log (0=debug, 1=info, 2=warning, 3=error)
function utils.setLogLvl(level) end

--
-- var : constantes et etats du moteur (lecture seule)
--

---@class var
var = {}

--- Position X de la souris
---@type integer
var.mouseX = 0

--- Position Y de la souris
---@type integer
var.mouseY = 0

--- Bouton gauche vient d'etre presse
---@type boolean
var.mouseLeftJustPressed = false

--- Bouton droit vient d'etre presse
---@type boolean
var.mouseRightJustPressed = false

--- Bouton gauche maintenu
---@type boolean
var.mouseLeftPressed = false

--- Bouton droit maintenu
---@type boolean
var.mouseRightPressed = false

--- Scroll vertical de la molette
---@type integer
var.mouseScrollVertical = 0

--- Scroll horizontal de la molette
---@type integer
var.mouseScrollHorizontal = 0

--- Delta time en secondes
---@type number
var.delta = 0

--- FPS actuels
---@type number
var.fps = 0

--- Nombre de frames ecoulees
---@type integer
var.frameCount = 0

--- Decalage X du viewport
---@type integer
var.offsetX = 0

--- Decalage Y du viewport
---@type integer
var.offsetY = 0

--- Largeur actuelle de la fenetre
---@type integer
var.currentWidth = 0

--- Hauteur actuelle de la fenetre
---@type integer
var.currentHeight = 0

--- Largeur de l'univers
---@type integer
var.universeWidth = 0

--- Hauteur de l'univers
---@type integer
var.universeHeight = 0

--- Fenetre minimisee
---@type boolean
var.windowMinimised = false

--
-- input : gestion du clavier et des manettes
--

---@class input
input = {}

--- Verifie si une touche vient d'etre pressee
---@param key string nom de la touche (ex: "a", "space", "escape")
---@return boolean
function input.keyJustPressed(key) end

--- Verifie si une touche est maintenue enfoncee
---@param key string nom de la touche
---@return boolean
function input.keyPressed(key) end

--- Affiche ou masque le curseur
---@param visible boolean true pour afficher, false pour masquer
function input.showCursor(visible) end

--- Verifie si un bouton de manette vient d'etre presse
---@param button string nom du bouton (ex: "a", "b", "x", "y")
---@param index integer index de la manette (0-3)
---@return boolean
function input.buttonJustPressed(button, index) end

--- Verifie si un bouton de manette est maintenu
---@param button string nom du bouton
---@param index integer index de la manette (0-3)
---@return boolean
function input.buttonPressed(button, index) end

--- Initialise une manette
---@param index integer index de la manette (0-3)
function input.initController(index) end

--- Recupere les valeurs des joysticks
---@param dead_zone number zone morte (0.0 - 1.0)
---@param index integer index de la manette (0-3)
---@return table {left_x, left_y, right_x, right_y, trigger_left, trigger_right}
function input.getJoysticks(dead_zone, index) end

--- Ferme un controller
---@param index integer index de la manette (0-3)
function input.closeController(index) end

--- Ferme un joystick
---@param index integer index de la manette (0-3)
function input.closeJoystick(index) end

--- Ferme le controller complet
---@param index integer index de la manette (0-3)
function input.closeTheController(index) end

--
-- image : graphiques, dessin et sprites
--

---@class image
image = {}

--- Dessine une image
---@param path string chemin de l'image
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@param flip? boolean retourner horizontalement (defaut: false)
---@param rotationP? number rotation pixel perfect en degres (defaut: 0)
---@param rotation? number rotation en degres (defaut: 0)
---@param alpha? integer opacite 0-255 (defaut: 255)
function image.draw(path, x, y, w, h, flip, rotationP, rotation, alpha) end

--- Dessine un rectangle plein
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@param r? integer rouge (defaut: 0)
---@param g? integer vert (defaut: 0)
---@param b? integer bleu (defaut: 0)
---@param a? integer opacite (defaut: 255)
function image.drawRect(x, y, w, h, r, g, b, a) end

--- Dessine un rectangle vide
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@param r? integer rouge (defaut: 0)
---@param g? integer vert (defaut: 0)
---@param b? integer bleu (defaut: 0)
---@param a? integer opacite (defaut: 255)
function image.drawRectFilled(x, y, w, h, r, g, b, a) end

--- Dessine une ligne
---@param x1 number debut X
---@param y1 number debut Y
---@param x2 number fin X
---@param y2 number fin Y
---@param r? integer rouge (defaut: 0)
---@param g? integer vert (defaut: 0)
---@param b? integer bleu (defaut: 0)
---@param a? integer opacite (defaut: 255)
function image.drawLine(x1, y1, x2, y2, r, g, b, a) end

--- Dessine un cercle vide
---@param x number centre X
---@param y number centre Y
---@param radius number rayon
---@param r? integer rouge (defaut: 0)
---@param g? integer vert (defaut: 0)
---@param b? integer bleu (defaut: 0)
---@param a? integer opacite (defaut: 255)
function image.drawCircle(x, y, radius, r, g, b, a) end

--- Dessine un cercle plein
---@param x number centre X
---@param y number centre Y
---@param radius number rayon
---@param r? integer rouge (defaut: 0)
---@param g? integer vert (defaut: 0)
---@param b? integer bleu (defaut: 0)
---@param a? integer opacite (defaut: 255)
function image.drawCircleFilled(x, y, radius, r, g, b, a) end

--- Dessine du texte
---@param font_path string chemin de la police (spritesheet)
---@param text string texte a afficher
---@param x number position X
---@param y number position Y
---@param scale number echelle
---@param flip? boolean retourner (defaut: false)
---@param spacing? number espacement (defaut: 1)
---@param rotationP? integer rotation pixel perfect (defaut: 0)
---@param rotation? integer rotation (defaut: 0)
---@param alpha? integer opacite (defaut: 255)
function image.drawText(font_path, text, x, y, scale, flip, spacing, rotationP, rotation, alpha) end

--- Definit l'icone de la fenetre
---@param path string chemin de l'image
function image.setIcon(path) end

--- Charge toutes les images d'un dossier
---@param folder string chemin du dossier
function image.loadImageFolder(folder) end

--- Libere les images chargees
function image.freeImageFolder() end

--- Dessine des particules
---@param x table positions X
---@param y table positions Y
---@param rotation table rotations
---@param a table opacites
---@param r table rouges
---@param g table verts
---@param b table bleus
function image.drawParticles(x, y, rotation, a, r, g, b) end

--- Dessine un sprite
---@param sprite Sprite le sprite
---@param index integer index de la frame
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@param flip? boolean retourner (defaut: false)
---@param rotation? integer rotation (defaut: 0)
---@param alpha? integer opacite (defaut: 255)
function image.drawSprite(sprite, index, x, y, w, h, flip, rotation, alpha) end

--- Cree un sprite a partir d'une spritesheet
---@param id string chemin de la spritesheet
---@param width integer largeur d'une frame
---@param height integer hauteur d'une frame
---@return Sprite
function image.Sprite(id, width, height) end

---@class Sprite
---@field width integer largeur d'une frame
---@field height integer hauteur d'une frame
local Sprite = {}

--
-- song : gestion audio
--

---@class song
song = {}

--- Joue un son
---@param path string chemin du fichier audio
---@param loop? integer nombre de boucles (defaut: 0)
---@param channel? integer canal audio (defaut: 1)
---@param volume? integer volume 0-128 (defaut: 124)
function song.playSound(path, loop, channel, volume) end

--- Arrete un son
---@param path string chemin du fichier audio
function song.stopSound(path) end

--- Reprend un son en pause
---@param path string chemin du fichier audio
function song.resumeSound(path) end

--- Charge tous les sons d'un dossier
---@param path string chemin du dossier
function song.loadSongFolder(path) end

--- Libere les sons charges
function song.freeSongFolder() end

--- Arrete un canal
---@param channel integer numero du canal
function song.stopChannel(channel) end

--- Met en pause un canal
---@param channel integer numero du canal
function song.pauseChannel(channel) end

--- Reprend un canal en pause
---@param channel integer numero du canal
function song.resumeChannel(channel) end

--
-- game : physique, blocs, entites et camera
--

---@class game
game = {}

--- Cree un bloc
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@param type integer type du bloc
---@return Block
function game.Block(x, y, w, h, type) end

--- Cree un conteneur de blocs vide
---@return Blocks
function game.Blocks() end

--- Cree une entite topdown
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@return EntityTopdown
function game.EntityTopdown(x, y, w, h) end

--- Cree une entite de platformer
---@param x number position X
---@param y number position Y
---@param w number largeur
---@param h number hauteur
---@param jumpPower? number force de saut (defaut: -150)
---@param gravity? number gravite (defaut: 300)
---@return EntityPlatformer
function game.EntityPlatformer(x, y, w, h, jumpPower, gravity) end

--- Cree une camera
---@param x number position X
---@param y number position Y
---@param smooth number facteur de lissage
---@param w integer largeur de la vue
---@param h integer hauteur de la vue
---@return Camera
function game.Camera(x, y, smooth, w, h) end

--- Charge des blocs depuis un fichier de carte
---@param path string chemin du fichier
---@param step_x integer pas en X (pixels)
---@param step_y integer pas en Y (pixels)
---@param separator string caractere de separation
---@param exclude integer type a exclure
---@return BlocksFromFile
function game.BlocksFromFile(path, step_x, step_y, separator, exclude) end

--- Calcule les collisions pour un jeu de platforme
---@param entity EntityPlatformer entite
---@param blocks Blocks|table blocs de collision
---@param max_fall_speed? number vitesse de chute max (defaut: 300)
---@param wall_correction? number correction murale (defaut: 150)
---@param ignore? table types de blocs a ignorer
---@return EntityPlatformer
function game.hitboxPlatformer(entity, blocks, max_fall_speed, wall_correction, ignore) end

--- Calcule les collisions pour un jeu en vue de dessus
---@param entity EntityTopdown entite
---@param blocks Blocks|table blocs de collision
---@param ignore? table types de blocs a ignorer
---@return EntityTopdown
function game.hitboxTopdown(entity, blocks, ignore) end

---@class Block
---@field x number position X
---@field y number position Y
---@field w number largeur
---@field h number hauteur
---@field type integer type du bloc
local Block = {}

---@class Blocks
local Blocks = {}

--- Ajoute un bloc
---@param block Block bloc a ajouter
function Blocks:add(block) end

--- Retourne le nombre de blocs
---@return integer
function Blocks:size() end

--- Recupere un bloc par index
---@param index integer
---@return Block
function Blocks:get(index) end

--- Iterateur sur les blocs
---@return function
function Blocks:pairs() end

---@class EntityTopdown
---@field x number position X
---@field y number position Y
---@field w number largeur
---@field h number hauteur
local EntityTopdown = {}

---@class EntityPlatformer
---@field x number position X
---@field y number position Y
---@field w number largeur
---@field h number hauteur
---@field gravity number force de gravite
---@field jumpPower number force de saut
---@field requestJump boolean demande de saut
---@field leftLock boolean bloque a gauche
---@field rightLock boolean bloque a droite
---@field inSky boolean en l'air
---@field speedY number vitesse verticale
local EntityPlatformer = {}

---@class Camera
---@field x number position X
---@field y number position Y
---@field smoothFactor number facteur de lissage
---@field w integer largeur
---@field h integer hauteur
local Camera = {}

--- Met a jour la camera vers une cible
---@param target_x number cible X
---@param target_y number cible Y
---@param dt number delta time
function Camera:update(target_x, target_y, dt) end

---@class BlocksFromFile
local BlocksFromFile = {}

--- Retourne le nombre de blocs
---@return integer
function BlocksFromFile:size() end
