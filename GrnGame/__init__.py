import os
from .chemin import retour_tout_chemin
from .utilitaires.systeme import renvoie_systeme
from .chargerlib import charger_module_nanobind
from .compilation import compilation_main
chemin_package, chemin_script, chemin_lib = retour_tout_chemin()
systeme =renvoie_systeme()
import sys
import ctypes


def compilation():
    compilation_main(renvoie_systeme(),chemin_lib)

called_program = os.path.basename(sys.argv[0])
COMMANDES_INTERDITES = {"GrnGame_app", "GrnGame_xmake","GrnGame_app.exe", "GrnGame_xmake.exe"}
dll_path = os.path.join(chemin_package, "VC_dll")
os.environ["PATH"] = dll_path + os.pathsep + os.environ.get("PATH", "")

jeu = None
if called_program not in COMMANDES_INTERDITES:
    try:
        if systeme == "windows" and   not(getattr(sys, 'frozen', False)):
            # Charger dynamiquement les DLL VC runtime
            for dll_name in ["VCRUNTIME140.dll", "VCRUNTIME140_1.dll"]:
                try:
                    ctypes.WinDLL(os.path.join(dll_path, dll_name))
                except OSError as e:
                    print(f"[!] Impossible de charger {dll_name} : {e}")


        # Charger le module Nanobind
        jeu = charger_module_nanobind("LibGrnGame", chemin_lib, os.path.join(chemin_package, "xmake"))
    except Exception as e:
        print("[!] Erreur lors du chargement de la lib, le moteur est désactivé.", e)


        
if jeu is not None and called_program not in COMMANDES_INTERDITES:
    from .nanobind.classe.utilitaires import utilitaires
    g = None
    utils = utilitaires(jeu)

    from .nanobind.classe.clavier import _clavier
    clavier = _clavier(utils)
    keyboard = clavier
    from .nanobind.classe.image import _image
    image = _image(utils)
    from .nanobind.classe.constantes import constante
    const = constante(utils)
    from .nanobind.classe.manette import _manette
    manette = _manette(utils)
    controller = manette
    from .nanobind.classe.son import _son
    son = _son(utils)
    song = son
    
    #symboles
    __all__ = ['utils', 'clavier','keyboard', 'const', 'image', 'manette','controller', 'son','song','platformer_2d']