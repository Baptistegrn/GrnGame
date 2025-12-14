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
        "rect:x,y,x2,y2,r,g,b,plein",
        2, 5, 1, 1, 1
    )

    # Centre
    cx, cy = 75, 75

    # Rectangles concentriques qui pulsent
    for i in range(8):
        taille = 15 + i * 10 + 8 * math.sin(t * 2 + i * 0.5)
        
        x = cx - taille // 2
        y = cy - taille // 2
        
        r = int(127 + 128 * math.sin(t + i * 0.7))
        g = int(127 + 128 * math.sin(t + i * 0.7 + 2))
        b = int(127 + 128 * math.sin(t + i * 0.7 + 4))
        
        # Alterner entre plein et creux
        plein = (i % 2 == 0)
        
        GrnGame.image.dessiner_rectangle(
            int(x), int(y), int(taille), int(taille),
            r, g, b, plein
        )

    # Rectangle central blanc
    GrnGame.image.dessiner_rectangle(
        cx - 5, cy - 5, 10, 10,
        255, 255, 255, False
    )

GrnGame.utils.init(
    largeur=150,
    hauteur=150,
    fps=60,
    coeff=4,
    update_func=update,
    nom_fenetre="Demo Rectangles",
    chemin_erreur="erreur/erreurs"
)