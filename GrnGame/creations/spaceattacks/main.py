import GrnGame as G

init_mannette = False
vies = 3
score = 0
game_over = False

vaisseau_x = 176
vaisseau_y = 340
vaisseau_degats = 0 
invincible_timer = 0 

projectiles = []  
projectile_anim_timer = 0    
projectile_anim_frame = 1  
tir_cooldown = 0.5 

monstres = [] 
monstre_spawn_timer = 0 
monstre_anim_timer = 0 
monstre_anim_frame = 1  

explosions = [] 

VAISSEAU_VITESSE = 200.0
PROJECTILE_VITESSE = 200.0
MONSTRE_VITESSE = 120.0
MONSTRE_SPAWN_DELAY = 0.4
INVINCIBLE_DURATION = 1.0
FIRE_RATE = 0.5

VAISSEAU_HITBOX = {'w': 38, 'h': 38, 'offset_x': 5, 'offset_y': 5}
MONSTRE_HITBOX = {'w': 47, 'h': 31, 'offset_x': 8, 'offset_y': 16}
PROJECTILE_HITBOX = {'w': 8, 'h': 24, 'offset_x': 12, 'offset_y': 4}


def collision_rect(x1, y1, w1, h1, x2, y2, w2, h2):
    return (x1 < x2 + w2 and x1 + w1 > x2 and
            y1 < y2 + h2 and y1 + h1 > y2)


def spawn_monstre():
    x = G.maths.random(0, 340)
    y = -70
    monstre = {'x': x, 'y': y}
    monstres.append(monstre)


def creer_explosion(x, y):
    explosion = {'x': x, 'y': y, 'frame': 1, 'timer': 0}
    explosions.append(explosion)
    G.son.jouer("assets/explosion.wav", boucle=1, canal=1,volume=10)


def tirer_projectile():
    global projectiles
    proj = {'x': vaisseau_x + 8, 'y': vaisseau_y - 10}
    projectiles.append(proj)
    G.son.jouer("assets/tir.wav", boucle=1, canal=2,volume=10)


def update_jeu():
    global init_mannette, vies, score, game_over
    global vaisseau_x, vaisseau_y, vaisseau_degats, invincible_timer
    global projectiles, projectile_anim_timer, projectile_anim_frame
    global monstres, monstre_spawn_timer, monstre_anim_timer, monstre_anim_frame
    global explosions, tir_cooldown
    
    dt = G.const.dt
    
    if G.clav.juste_presser("F3"):
        G.utils.redimensionner_fenetre()
    
    if not init_mannette:
       init_mannette= G.man.init()
    
    G.image.dessiner("assets/fond.png", 0, 0, 400, 400)
    
    if game_over:
        G.image.dessiner_mot("assets/police", "GAME OVER", 130, 200, 2, 3)
        G.image.dessiner_mot("assets/police", f"SCORE: {score}", 165, 250, 1, 3)
        G.image.dessiner_mot("assets/police", "APPUIE B RESTART", 140, 300, 1, 2)
        
        if G.man.juste_presse("B"):
            vies = 3
            score = 0
            game_over = False
            vaisseau_x = 176
            vaisseau_y = 340
            vaisseau_degats = 0
            invincible_timer = 0
            projectiles.clear()
            monstres.clear()
            explosions.clear()
        return
    
    if invincible_timer > 0:
        invincible_timer -= dt
        if invincible_timer < 0:
            invincible_timer = 0
    
    joysticks = G.man.renvoie_joysticks()
    joy_x = joysticks[0]
    joy_y = joysticks[1]
    
    vaisseau_x += joy_x * VAISSEAU_VITESSE * dt
    vaisseau_y += joy_y * VAISSEAU_VITESSE * dt
    vaisseau_x = max(0, min(vaisseau_x, 352))
    vaisseau_y = max(0, min(vaisseau_y, 352))
    
    if tir_cooldown > 0:
        tir_cooldown -= dt
    
    if G.man.enfoncee("A") and tir_cooldown <= 0:
        tirer_projectile()
        tir_cooldown = FIRE_RATE
    
    projectile_anim_timer += dt
    if projectile_anim_timer > 0.15:
        projectile_anim_timer = 0
        projectile_anim_frame = (projectile_anim_frame % 3) + 1
    
    projectiles_a_supprimer = []
    for proj in projectiles:
        proj['y'] -= PROJECTILE_VITESSE * dt
        if proj['y'] < -20:
            projectiles_a_supprimer.append(proj)
    
    for proj in projectiles_a_supprimer:
        projectiles.remove(proj)
    
    monstre_spawn_timer += dt
    if monstre_spawn_timer > MONSTRE_SPAWN_DELAY:
        monstre_spawn_timer = 0
        spawn_monstre()
    
    monstre_anim_timer += dt
    if monstre_anim_timer > 0.2:
        monstre_anim_timer = 0
        monstre_anim_frame = (monstre_anim_frame % 6) + 1
    
    monstres_a_supprimer = []
    for monstre in monstres:
        monstre['y'] += MONSTRE_VITESSE * dt
        if monstre['y'] > 400:
            monstres_a_supprimer.append(monstre)
            continue
        
        if invincible_timer == 0 and collision_rect(
            monstre['x'] + MONSTRE_HITBOX['offset_x'], 
            monstre['y'] + MONSTRE_HITBOX['offset_y'], 
            MONSTRE_HITBOX['w'], 
            MONSTRE_HITBOX['h'],
            vaisseau_x + VAISSEAU_HITBOX['offset_x'], 
            vaisseau_y + VAISSEAU_HITBOX['offset_y'], 
            VAISSEAU_HITBOX['w'], 
            VAISSEAU_HITBOX['h']
        ):
            vies -= 1
            vaisseau_degats = min(vaisseau_degats + 1, 3)
            invincible_timer = INVINCIBLE_DURATION
            creer_explosion(monstre['x'], monstre['y'])
            monstres_a_supprimer.append(monstre)
            G.son.jouer("assets/degat.wav", boucle=1, canal=3,volume=10)
            if vies <= 0:
                game_over = True
            continue
        
        for proj in projectiles:
            if collision_rect(
                monstre['x'] + MONSTRE_HITBOX['offset_x'], 
                monstre['y'] + MONSTRE_HITBOX['offset_y'], 
                MONSTRE_HITBOX['w'], 
                MONSTRE_HITBOX['h'],
                proj['x'] + PROJECTILE_HITBOX['offset_x'], 
                proj['y'] + PROJECTILE_HITBOX['offset_y'], 
                PROJECTILE_HITBOX['w'], 
                PROJECTILE_HITBOX['h']
            ):
                score += 100
                creer_explosion(monstre['x'], monstre['y'])
                monstres_a_supprimer.append(monstre)
                projectiles_a_supprimer.append(proj)
                break
    
    for monstre in monstres_a_supprimer:
        if monstre in monstres:
            monstres.remove(monstre)
    for proj in projectiles_a_supprimer:
        if proj in projectiles:
            projectiles.remove(proj)
    
    explosions_a_supprimer = []
    for exp in explosions:
        exp['timer'] += dt
        if exp['timer'] > 0.1:
            exp['timer'] = 0
            exp['frame'] += 1
            if exp['frame'] > 3:
                explosions_a_supprimer.append(exp)
    
    for exp in explosions_a_supprimer:
        if exp in explosions:
            explosions.remove(exp)
    
    for exp in explosions:
        G.image.dessiner(f"assets/explosion {exp['frame']}.png", exp['x'], exp['y'], 62, 62)
    
    for monstre in monstres:
        G.image.dessiner(f"assets/monstre {monstre_anim_frame}.png", monstre['x'], monstre['y'], 62, 62)
    
    for proj in projectiles:
        G.image.dessiner(f"assets/projectile {projectile_anim_frame}.png", proj['x'], proj['y'], 32, 32)
    
    if invincible_timer == 0 or int(invincible_timer * 10) % 2 == 0:
        frame_vaisseau = min(vaisseau_degats + 1, 4)
        G.image.dessiner(f"assets/vaisseau {frame_vaisseau}.png", vaisseau_x, vaisseau_y, 48, 48)
    
    G.image.dessiner_mot("assets/police", f"VIES: {vies}", 10, 10, 1, 2)
    G.image.dessiner_mot("assets/police", f"SCORE: {score}", 300, 10, 1, 2)


G.utils.init(
    largeur=400,
    hauteur=400,
    fps=64,
    coeff=2,
    chemin_image="assets",
    chemin_son="assets",

    bande_noir=True,
    update_func=update_jeu,
    nom_fenetre="Space Attacks",chemin_erreur="err.log"
)
