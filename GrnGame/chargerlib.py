import os
import shutil
import subprocess
import importlib.util

def charger_module_nanobind(nom_module, chemin_lib, chemin_xmake):
    # verifie que le chemin vers lib existe
    if os.path.isfile(chemin_lib):
        try:
            # essaye de charger 
            spec = importlib.util.spec_from_file_location(nom_module, chemin_lib)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)

            print(f"[+] Module nanobind chargé : {nom_module}")
            return module

        except Exception as e:
            print(f"[!] Impossible de charger le module : {e}")
            print("[i] La lib est peut-être corrompue, suppression...")
            try:
                # suppression bug xmake pour remettre une version qui fonctionne sur los
                os.remove(chemin_lib)
            except Exception as err:
                # erreur final retur
                print(f"[!] Impossible de supprimer la lib : {err}")
                return None

    # soit absente soit supprimer donc on compile
    print(f"[!] '{chemin_lib}' absent. Compilation requise.")
    # on cherche xmake
    xmake_path = shutil.which("xmake")

    if not xmake_path:
        print("[!] xmake introuvable.")
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
    # on reesaye de charger la lib
    if os.path.isfile(chemin_lib):
        try:
            spec = importlib.util.spec_from_file_location(nom_module, chemin_lib)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            print("[+] Module nanobind chargé après compilation.")
            return module
        except Exception as e:
            print(f"[!] Impossible de charger après compilation : {e}")
            return None

    print("[!] La lib n’a pas été générée.")
    return None
