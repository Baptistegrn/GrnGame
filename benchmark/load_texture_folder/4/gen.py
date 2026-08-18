import argparse
import random
from PIL import Image

def generateur_images_palette(largeur, hauteur, nb_couleurs, nb_images):
    """
    Génère des images où chaque pixel est coloré en piochant 
    parmi un nombre défini de couleurs aléatoires.
    """
    nombre_total_pixels = largeur * hauteur

    for _ in range(nb_images):
        # 1. Générer la palette
        palette = [
            (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
            for _ in range(nb_couleurs)
        ]
        
        # 2. Remplir l'image
        donnees_pixels = random.choices(palette, k=nombre_total_pixels)
        
        # 3. Créer et retourner l'image
        img = Image.new('RGB', (largeur, hauteur))
        img.putdata(donnees_pixels)
        
        yield img

def main():
    # Configuration de l'analyseur d'arguments
    parser = argparse.ArgumentParser(description="Générateur d'images avec une palette de couleurs aléatoires.")
    
    # Ajout des arguments de la ligne de commande
    parser.add_argument("-W", "--largeur", type=int, required=True, help="Largeur de l'image en pixels")
    parser.add_argument("-H", "--hauteur", type=int, required=True, help="Hauteur de l'image en pixels")
    parser.add_argument("-c", "--couleurs", type=int, required=True, help="Nombre de couleurs aléatoires dans la palette")
    parser.add_argument("-n", "--nombre", type=int, default=1, help="Nombre d'images à générer (défaut: 1)")
    parser.add_argument("-p", "--prefixe", type=str, default="image_palette", help="Préfixe pour le nom du fichier (défaut: image_palette)")

    # Lecture des arguments passés dans la console
    args = parser.parse_args()

    # Initialisation du générateur avec les paramètres de la console
    generateur = generateur_images_palette(
        largeur=args.largeur, 
        hauteur=args.hauteur, 
        nb_couleurs=args.couleurs, 
        nb_images=args.nombre
    )

    # Génération et sauvegarde
    print(f"Génération de {args.nombre} image(s) de {args.largeur}x{args.hauteur} avec {args.couleurs} couleurs...")
    
    for i, image in enumerate(generateur):
        nom_fichier = f"{args.prefixe}_{i + 1}.png"
        image.save(nom_fichier)
        print(f" -> {nom_fichier} sauvegardée.")

if __name__ == "__main__":
    main()