import os
import sys
import shutil
import ctypes
import subprocess


def charger_lib(chemin_lib, chemin_xmake):
    # 1. Vérifier si la lib existe
    if os.path.isfile(chemin_lib):
        try:
            lib = ctypes.CDLL(chemin_lib)
            if not getattr(sys, 'frozen', False):
                print(f"[+] Librairie chargée : {chemin_lib}")
            return lib

        except OSError as e:
            print(f"[!] Impossible de charger la librairie : {e}")
            print("[i] La librairie est probablement corrompue ou incompatible. Suppression...")

            try:
                os.remove(chemin_lib)
                print("[i] Ancienne lib supprimée.")
            except Exception as err:
                print(f"[!] Impossible de supprimer la lib : {err}")
                return None  # impossible de corriger  on arrête

    # 2. Si on arrive ici : lib absente ou supprimée  on tente compilation
    print(f"[!] La librairie '{chemin_lib}' est absente. Compilation requise.")

    xmake_path = shutil.which("xmake")
    if not xmake_path:
        print("[!] xmake introuvable. Impossible de compiler la lib.")
        return None

    print("[i] xmake trouvé. Compilation en cours...")

    try:
        result = subprocess.run(
            [xmake_path, "-y"],
            cwd=chemin_xmake,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        print(result.stdout)
        print("[+] Compilation réussie.")

    except subprocess.CalledProcessError as e:
        print("[!] Échec de la compilation avec xmake.")
        print(e.stdout)
        print(e.stderr)
        return None

    # 3. Charger la lib compilée
    if os.path.isfile(chemin_lib):
        try:
            lib = ctypes.CDLL(chemin_lib)
            print("[+] lib chargée après compilation.")
            return lib
        except OSError as e:
            print(f"[!] Impossible de charger la librairie après compilation : {e}")
            return None

    print("[!] La DLL n'a pas été générée après compilation.")
    return None
