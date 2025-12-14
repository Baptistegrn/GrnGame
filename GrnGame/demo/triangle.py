import GrnGame
import math

t = 0

def update():
    global t
    GrnGame.utils.afficher_curseur(True)
    GrnGame.utils.colorier(10, 10, 15)

    t += GrnGame.const.dt

    # Titre
    GrnGame.image.dessiner_mot(
        "./assets/police2",
        "triangle:x,y,x1,y1,r,g,b,plein",
        0, 5, 1, 1, 1
    )

    # Centre
    cx, cy = 75-10, 75-10
    
    # Triangles qui orbitent et pulsent
    for i in range(6):
        angle = t + i * (math.pi / 3)
        distance = 35
        
        # Position du triangle
        pos_x = cx + int(distance * math.cos(angle))
        pos_y = cy + int(distance * math.sin(angle))
        
        # Taille qui pulse
        w = int(15 + 8 * math.sin(t * 2 + i))
        h = int(15 + 8 * math.sin(t * 2 + i))
        
        r = int(127 + 128 * math.sin(t + i))
        g = int(127 + 128 * math.cos(t + i + 1))
        b = int(127 + 128 * math.sin(t + i + 3))
        
        # Alterner entre plein et creux
        plein = (i % 2 == 1)
        
        GrnGame.image.dessiner_triangle(
            pos_x, pos_y, w, h,
            r, g, b, plein
        )
    
    # Triangle central fixe
    GrnGame.image.dessiner_triangle(
        cx, cy, 15, 15,
        255, 255, 255, False
    )

GrnGame.utils.init(
    largeur=150,
    hauteur=150,
    fps=60,
    coeff=4,
    update_func=update,
    nom_fenetre="Demo Triangles",
    chemin_erreur="erreur/erreurs"
)