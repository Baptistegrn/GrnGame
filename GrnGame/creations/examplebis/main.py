import GrnGame

camera_x = 0.0
camera_y = 0.0

# Variables du joueur
joueur_x = 80.0
joueur_y = 50.0
joueur_vitesse_y = 0.0
joueur_en_air = True
joueur_largeur = 12.0
joueur_hauteur = 12.0
g,d = False , False
frame = 1
sens = 0
grille = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1],
    [1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
]

# Conversion de la grille en liste de blocs pour la physique
def grille_vers_blocs():
    taille_bloc = 16.0
    blocs = []
    for y in range(len(grille)):
        for x in range(len(grille[y])):
            if grille[y][x] == 1:
                blocs.append((x * taille_bloc, y * taille_bloc, taille_bloc, taille_bloc))
    return blocs

blocs = grille_vers_blocs()

def dessiner_grille():
    taille_bloc = 16.0
    
    # Listes pour le batch
    xs = []
    ys = []
    ws = []
    hs = []
    rs = []
    gs = []
    bs = []
    
    for y in range(len(grille)):
        for x in range(len(grille[y])):
            if grille[y][x] == 1:
                bloc_x = x * taille_bloc
                bloc_y = y * taille_bloc
                ecran_x = bloc_x - camera_x
                ecran_y = bloc_y - camera_y
                
                # Bloc principal (gris foncé)
                xs.append(ecran_x)
                ys.append(ecran_y)
                ws.append(taille_bloc)
                hs.append(taille_bloc)
                rs.append(80)
                gs.append(80)
                bs.append(80)
                
                # Bordure haut (gris clair)
                xs.append(ecran_x)
                ys.append(ecran_y)
                ws.append(taille_bloc)
                hs.append(1.0)
                rs.append(120)
                gs.append(120)
                bs.append(120)
                
                # Bordure gauche (gris clair)
                xs.append(ecran_x)
                ys.append(ecran_y)
                ws.append(1.0)
                hs.append(taille_bloc)
                rs.append(120)
                gs.append(120)
                bs.append(120)
                
                # Bordure bas (gris foncé)
                xs.append(ecran_x)
                ys.append(ecran_y + taille_bloc - 1.0)
                ws.append(taille_bloc)
                hs.append(1.0)
                rs.append(40)
                gs.append(40)
                bs.append(40)
                
                # Bordure droite (gris foncé)
                xs.append(ecran_x + taille_bloc - 1.0)
                ys.append(ecran_y)
                ws.append(1.0)
                hs.append(taille_bloc)
                rs.append(40)
                gs.append(40)
                bs.append(40)
    
    # Dessiner tout d'un coup
    if len(xs) > 0:
        GrnGame.image.dessiner_forme_batch(
            xs=xs,
            ys=ys,
            ws=ws,
            hs=hs,
            rs=rs,
            gs=gs,
            bs=bs
        )

def update():
    global camera_x, camera_y
    global joueur_x, joueur_y, joueur_vitesse_y, joueur_en_air,g,d,frame,sens
    if GrnGame.const.time %20 ==0:
        frame+=1
        if frame>5:
            frame= 1
    # Déplacement horizontal du joueur
    vitesse_deplacement = 100.0
    if GrnGame.clavier.enfoncee("q") and not g:
        joueur_x -= vitesse_deplacement * GrnGame.const.dt
        sens=0
    if GrnGame.clavier.enfoncee("d")and not d:
        sens=1
        joueur_x += vitesse_deplacement * GrnGame.const.dt
    
    # Physique platformer avec détection de collision
    
    joueur_x, joueur_y, joueur_vitesse_y, joueur_en_air, g, d = \
    GrnGame.utils.platformer_2d(
        dt=GrnGame.const.dt,
        pos_x=joueur_x,
        pos_y=joueur_y,
        vitesse_y=joueur_vitesse_y,
        en_air=joueur_en_air,
        larg_joueur=joueur_largeur,
        haut_joueur=joueur_hauteur,
        blocs=blocs,
        gravite=600.0,
        force_saut=-250.0,
        activer_saut = (GrnGame.clavier.juste_presse("z")
                        or GrnGame.clavier.juste_presse("space"))
    )


    # Caméra suit le joueur
    camera_x = joueur_x - 80.0 + joueur_largeur / 2.0
    camera_y = joueur_y - 80.0 + joueur_hauteur / 2.0
    
    # Basculer plein écran
    if GrnGame.clavier.juste_presse("f4"):
        GrnGame.utils.redimensionner_fenetre()
    if GrnGame.clavier.juste_presse("f5"):
        GrnGame.utils.stopper_jeu()
    # Dessiner la grille
    dessiner_grille()
    
    # Dessiner le joueur (rouge)
    GrnGame.image.dessiner(
        lien=f"./assets/_player_{frame}.png",
        x=joueur_x - camera_x,
        y=joueur_y - camera_y,
        w=joueur_largeur,
        h=joueur_hauteur,
        sens=sens,
        rotation=0,
        transparence=255,
    )
    
    GrnGame.utils.colorier(10, 30, 255)

GrnGame.utils.init(
    largeur=160,
    hauteur=160,
    fps=128,
    coeff=3,
    chemin_image="./assets",
    chemin_son="./assets",
    bande_noir=False,
    update_func=update,
    nom_fenetre="Test Platformer 16x16",
    chemin_erreur="errer/errs.log"
)