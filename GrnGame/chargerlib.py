import subprocess
import ctypes
import os
import sys

import shutil

def charger_lib(chemin_lib, chemin_xmake):
    if os.path.isfile(chemin_lib):
        try:

            lib = ctypes.CDLL(chemin_lib)
            if getattr(sys, 'frozen', False):
                pass
            else:
                print(f"[+] Librairie chargée : {chemin_lib}")
            return lib
        except OSError as e:
            print(f"[!] Impossible de charger la librairie : {e}")
            return None

    print(f"[!] La librairie '{chemin_lib}' est absente.")

    xmake_path = shutil.which("xmake")
    if not xmake_path:
        print("[!] xmake introuvable. Impossible de compiler la DLL veuillez executer la commande GrnGame_xmake et relancer la console.")
        return None

    print("[i] xmake trouvé. Compilation en cours...")

    try:
        result = subprocess.run(
            [xmake_path]+ ["-y"],
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
    if os.path.isfile(chemin_lib):
        try:
            lib = ctypes.CDLL(chemin_lib)
            return lib
        except OSError as e:
            print(f"[!] Impossible de charger la librairie après compilation : {e}")
            return None
    else:
        print("[!] La DLL n'a pas été générée même après compilation.")
        return None