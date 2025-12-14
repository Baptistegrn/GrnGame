import GrnGame
import math
t = 0

def update():
    global t
    GrnGame.utils.afficher_curseur(True)
    GrnGame.utils.colorier(10, 10, 15)

    t += GrnGame.const.dt

    GrnGame.image.dessiner_mot(
        "./assets/police2",
        "cercle:x,y,rayon,r,g,b,plein",
        2, 5, 1, 1, 1
    )

    cx, cy = 75, 75      
    orbite = 45         

    for i in range(8):
        angle = t + i * (math.pi / 4)
        rayon = 10 + 7 * math.sin(t + i)

        x = cx + int(orbite * math.cos(angle))
        y = cy + int(orbite * math.sin(angle))

        r = int(127 + 128 * math.sin(t + i))
        g = int(127 + 128 * math.sin(t + i + 2))
        b = int(127 + 128 * math.sin(t + i + 4))

        plein = (i % 2 == 0)

        GrnGame.image.dessiner_cercle(
            x, y, int(rayon),
            r=r, g=g, b=b, plein=plein
        )

    rayon_central = 15 + 7 * math.sin(t * 2)
    GrnGame.image.dessiner_cercle(  
        cx, cy, int(rayon_central),
        r=255, g=255, b=255, plein=False
    )

GrnGame.utils.init(
    largeur=150,
    hauteur=150,
    fps=60,
    coeff=4,
    update_func=update,
    nom_fenetre="Cercles demo",
    chemin_erreur="erreur/erreurs"
)
