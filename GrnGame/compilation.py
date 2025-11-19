import os
import sys
import subprocess

def compilation_main(systeme, chemin_lib):
    import argparse
    import shutil

    parser = argparse.ArgumentParser(
        description="Compile un script Python en exécutable avec Grngame",
        usage="GrnGame_app script.py [--noconsole] [--icon chemin/vers/icone.ico]"
    )
    parser.add_argument("script", help="Fichier Python à compiler")
    parser.add_argument("--noconsole", action="store_true", 
                       help="Cache la console (Windows uniquement)")
    parser.add_argument("--icon", type=str, default=None,
                       help="Chemin vers l'icône (.ico pour Windows, .icns pour macOS)")
    
    args = parser.parse_args(sys.argv[1:])
    
    script_to_compile = args.script

    if not os.path.exists(script_to_compile):
        print(f"Erreur : Le fichier '{script_to_compile}' n'existe pas.")
        return 1

    # Separateur entre Windows et Linux
    separator = ";" if systeme == "windows" else ":"

    exe_name = os.path.splitext(os.path.basename(script_to_compile))[0]
    if systeme == "windows":
        exe_name_sans_exe = exe_name
        exe_name += ".exe"


    cmd = [
        "pyinstaller",
        "--onefile",
        "--clean",
        f"--add-binary={chemin_lib}{separator}."
    ]

    if args.noconsole:
        if systeme == "windows":
            cmd.append("--noconsole")
            print("Mode sans console activé")
        else:
            print("Avertissement : --noconsole est ignoré (uniquement Windows)")

    if args.icon:
        if os.path.exists(args.icon):
            cmd.append(f"--icon={args.icon}")
            print(f"Icône ajoutée : {args.icon}")
        else:
            print(f"Avertissement : L'icône '{args.icon}' n'existe pas, elle sera ignorée.")

    # Ajouter l'option pour mettre l'exécutable dans le dossier courant
    cmd.append(f"--distpath=.")
    cmd.append(script_to_compile)
    
    print("\nCommande de compilation :")
    print(" ".join(cmd))
    print()
    
    try:
        if systeme == "windows":
            subprocess.run(cmd, check=True, creationflags=subprocess.CREATE_NEW_CONSOLE)
        else:
            subprocess.run(cmd, check=True)
        exe_path = os.path.join(".", exe_name)
        print(f"\nExécutable créé : {os.path.abspath(exe_path)}")
        
    except subprocess.CalledProcessError as e:
        print(f"\nÉchec de la compilation : {e}")
        return 1
    except FileNotFoundError:
        print("\nErreur : PyInstaller n'est pas installé.")
        print("Installez-le avec : pip install pyinstaller")
        return 1
    
    
    # Supprimer build, dist et .spec si présents
    for item in ["build", f"{exe_name}.spec",f"{exe_name_sans_exe}.spec"]:
        if os.path.exists(item):
            if os.path.isdir(item):
                shutil.rmtree(item)
                print(f"Dossier supprimé : {item}")
            else:
                os.remove(item)
                print(f"Fichier supprimé : {item}")
    return 0