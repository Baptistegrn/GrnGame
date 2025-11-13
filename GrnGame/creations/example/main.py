import GrnGame as G

init_mannette = False


persos = [
    {"x": 40, "y": 50, "vie": 3, "etat": "idle", "direction": "front", "frame": 1, "timer": 0, "hurt_timer": 0},
    {"x": 120, "y": 50, "vie": 3, "etat": "idle", "direction": "front", "frame": 1, "timer": 0, "hurt_timer": 0}
]

PERSO_VITESSE = 60.0
frames = {
    "idle": {"front": [1,2], "right": [3,4], "back": [5,6], "left": [3,4]},
    "walk": {"front": [7,8,9,10], "right": [11,12,13,14], "back": [15,16,17,18], "left": [11,12,13,14]},
    "hurt": {"front": [19,20], "right": [21,22], "back": [23,24], "left": [21,22]},
    "death": {"front": [25,26,27], "right": [28,29,30], "back": [31,32,33], "left": [28,29,30]}
}

def dessiner_perso(perso):
    sens = 1 if perso["direction"] == "left" else 0
    frame_list = frames[perso["etat"]][perso["direction"]]
    frame_index = (perso["frame"] - 1) % len(frame_list)
    frame_num = frame_list[frame_index]
    G.image.dessiner(f"./assets/{frame_num}.png", perso["x"], perso["y"], 32, 32, sens=sens)

def infliger_degats(perso):
    """Inflige 1 dégât si pas déjà en hurt ou mort."""
    if perso["etat"] not in ["hurt", "death"]:
        perso["vie"] -= 1
        perso["etat"] = "hurt"
        perso["frame"] = 1
        perso["hurt_timer"] = 0.6  
        if perso["vie"] <= 0:
            perso["etat"] = "death"


def update_jeu():
    global init_mannette

    if G.clav.juste_presser("F3"):
        G.utils.redimensionner_fenetre()
    
    dt = G.const.dt

    if not init_mannette:
        init_mannette = G.man.init()
    G.image.dessiner("./assets/fond.png", 0, 0, 192, 151)
    joysticks = G.man.renvoie_joysticks()
    joy0_x, joy0_y = joysticks[0], joysticks[1]
    trigger_gauche = joysticks[4]
    joy1_x, joy1_y = joysticks[2], joysticks[3]
    trigger_droit = joysticks[5] if len(joysticks) > 5 else 0
    perso0 = persos[0]
    if perso0["etat"] not in ["hurt", "death"]:
        if joy0_x != 0 or joy0_y != 0:
            longueur = (joy0_x**2 + joy0_y**2) ** 0.5
            if longueur > 1:
                joy0_x /= longueur
                joy0_y /= longueur

            perso0["etat"] = "walk"
            if abs(joy0_x) > abs(joy0_y):
                perso0["direction"] = "right" if joy0_x > 0 else "left"
            else:
                perso0["direction"] = "back" if joy0_y < 0 else "front"

            perso0["x"] += joy0_x * PERSO_VITESSE * dt
            perso0["y"] += joy0_y * PERSO_VITESSE * dt
            perso0["x"] = max(0, min(perso0["x"], 160))
            perso0["y"] = max(0, min(perso0["y"], 120))
        else:
            perso0["etat"] = "idle"
    if trigger_gauche > 0.5:
        infliger_degats(perso0)
    perso1 = persos[1]
    if perso1["etat"] not in ["hurt", "death"]:
        if joy1_x != 0 or joy1_y != 0:
            longueur = (joy1_x**2 + joy1_y**2) ** 0.5
            if longueur > 1:
                joy1_x /= longueur
                joy1_y /= longueur

            perso1["etat"] = "walk"
            if abs(joy1_x) > abs(joy1_y):
                perso1["direction"] = "right" if joy1_x > 0 else "left"
            else:
                perso1["direction"] = "back" if joy1_y < 0 else "front"

            perso1["x"] += joy1_x * PERSO_VITESSE * dt
            perso1["y"] += joy1_y * PERSO_VITESSE * dt
            perso1["x"] = max(0, min(perso1["x"], 160))
            perso1["y"] = max(0, min(perso1["y"], 120))
        else:
            perso1["etat"] = "idle"

    if trigger_droit > 0.5:
        infliger_degats(perso1)

    for p in persos:
        p["timer"] += dt
        if p["timer"] > 0.3:
            p["timer"] = 0
            p["frame"] += 1
        if p["etat"] == "hurt":
            p["hurt_timer"] -= dt
            if p["hurt_timer"] <= 0:
                if p["vie"] > 0:
                    p["etat"] = "idle"
                else:
                    p["etat"] = "death"

        dessiner_perso(p)
        G.image.dessiner_mot("./assets/police", f"VIES:{p['vie']}", p["x"] + 5, p["y"] + 3, 0.50, 0.65)

G.utils.init(
    largeur=192,
    hauteur=151,
    fps=120,
    coeff=4,
    chemin_image="./assets",
    chemin_son="./assets",
    bande_noir=True,
    update_func=update_jeu,
    nom_fenetre="Prototype Perso Joystick"
)
