import GrnGame
import math
import time

# Texte à afficher
texte = [
    "QUETE ANCIENNE",
    "Le Cercle s'est eveille.",
    "Huit orbes dansent",
    "autour du coeur blanc.",
    "Trace le symbole.",
    "Observe le rythme.",
    "Quand les couleurs s'accordent,",
    "le passage s'ouvre.",
    "Appuie sur [ESPACE] pour continuer."
]

# Variables pour le texte progressif
index_ligne = 0        # Ligne actuelle
index_caractere = 0    # Caractère actuel dans la ligne
delai = 2              # Nombre de frames entre chaque caractère (ajuste selon vitesse)
compteur = 0

def afficher_texte_progressif(lignes, x, y, couleur=(1,1,1), hauteur=12):
    global index_ligne, index_caractere, compteur
    
    for i in range(index_ligne):
        # Affiche les lignes déjà complètes
        GrnGame.image.dessiner_mot("./assets/police2", lignes[i], x, y + i*hauteur,1,1,1)
    
    if index_ligne < len(lignes):
        # Affiche la ligne en cours progressivement
        compteur += 1
        if compteur >= delai:
            index_caractere += 1
            compteur = 0
        
        ligne_courante = lignes[index_ligne][:index_caractere]
        GrnGame.image.dessiner_mot("./assets/police2", ligne_courante, x, y + index_ligne*hauteur,1,1,1)
        
        # Si la ligne est finie, passer à la suivante
        if index_caractere >= len(lignes[index_ligne]):
            index_ligne += 1
            index_caractere = 0

def update():
    GrnGame.utils.colorier(10, 10, 15)
    afficher_texte_progressif(texte, 10, 10)

GrnGame.utils.init(
    largeur=320,
    hauteur=180,
    fps=30,
    coeff=2,
    update_func=update,
    nom_fenetre="Texte demo",
    chemin_erreur="erreur/erreurs"
)
