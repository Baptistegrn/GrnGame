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
        "ligne:x,y,x2,y2,r,g,b",
        2, 5, 1, 1, 1
    )

    # Centre de rotation
    cx, cy = 75, 75
    longueur = 35

    # Lignes qui tournent comme une roue
    for i in range(12):
        angle = t + i * (math.pi / 6)
        
        x1 = cx
        y1 = cy
        x2 = cx + int(longueur * math.cos(angle))
        y2 = cy + int(longueur * math.sin(angle))
        
        r = int(127 + 128 * math.sin(t + i))
        g = int(127 + 128 * math.sin(t + i + 2))
        b = int(127 + 128 * math.sin(t + i + 4))
        
        GrnGame.image.dessiner_ligne(x1, y1, x2, y2, r, g, b)

    # Cercle central (point de référence)
    GrnGame.image.dessiner_cercle(cx, cy, 3, 255, 255, 255, True)

GrnGame.utils.init(
    largeur=150,
    hauteur=150,
    fps=60,
    coeff=4,
    update_func=update,
    nom_fenetre="Demo Lignes",
    chemin_erreur="erreur/erreurs"
)