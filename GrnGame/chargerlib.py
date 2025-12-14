import os
import shutil
import subprocess
import importlib.util

def charger_module_nanobind(nom_module, chemin_lib, chemin_xmake):
    #print(chemin_lib,chemin_xmake)
    def _charger_lib(path):
        spec = importlib.util.spec_from_file_location(nom_module, path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module

    # Essaye de charger si existante
    if os.path.isfile(chemin_lib):
        try:
            return _charger_lib(chemin_lib)
        except Exception as e:
            print(f"[!] Impossible de charger le module : {e}")
            print("[i] La lib est peut-être corrompue, suppression...")
            try:
                os.remove(chemin_lib)
            except Exception as err:
                print(f"[!] Impossible de supprimer la lib : {err}")

    # Compilation si absente ou supprimée
    print(f"[!] '{chemin_lib}' absent. Compilation requise.")
    xmake_path = shutil.which("xmake")
    if not xmake_path:
        print("[!] xmake introuvable. Veuillez exécuter GrnGame_xmake et relancer la console.")
        return None

    print("[i] Compilation en cours...")
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
        print("[+] Compilation OK.")
    except subprocess.CalledProcessError as e:
        print("[!] Erreur compilation.")
        print(e.stdout)
        print(e.stderr)
        return None

    # Rechargement après compilation
    if os.path.isfile(chemin_lib):
        try:
            return _charger_lib(chemin_lib)
        except Exception as e:
            print(f"[!] Impossible de charger après compilation : {e}")
            return None

    print("[!] La lib n’a pas été générée.")
    return None
