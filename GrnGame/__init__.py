
import os
from .chemin import retour_tout_chemin
from .utilitaires.systeme import renvoie_systeme
from .chargerlib import charger_lib
from .ctypes.structures import *
from .ctypes.signatures import configurer_signatures

from .compilation import compilation_main
chemin_package, chemin_script, chemin_lib = retour_tout_chemin()
import sys

from .hitbox.hitbox2dplatformer import platformer_2d
def compilation():
    compilation_main(renvoie_systeme(),chemin_lib)

called_program = os.path.basename(sys.argv[0])
COMMANDES_INTERDITES = {"GrnGame_app", "GrnGame_xmake","GrnGame_app.exe", "GrnGame_xmake.exe"}


jeu = None
if called_program not in COMMANDES_INTERDITES:
    try:
        jeu = charger_lib(chemin_lib, os.path.join(chemin_package, "xmake"))
        if jeu is not None:
            configurer_signatures(jeu)
        else:
            print("[!] La lib n'a pas pu être chargée, le moteur est désactivé.")
    except Exception:
        print("[!] Erreur lors du chargement de la lib, le moteur est désactivé.")

        
if jeu is not None and called_program not in COMMANDES_INTERDITES:
    from .ctypes.classe.utilitaires import utilitaires
    g = None
    utils = utilitaires(jeu)
    from .ctypes.classe.clavier import clavier
    clav = clavier(jeu,utils)

    from .ctypes.classe.constantes import constante
    const = constante(jeu,utils)

    from .ctypes.classe.image import _image
    image = _image(jeu,utils)

    from .ctypes.classe.manette import manette
    man = manette(jeu,utils)

    from .ctypes.classe.maths import _maths
    maths = _maths(jeu,utils)

    from .ctypes.classe.son import _son
    son = _son(jeu,utils)
    __all__ = ['utils', 'clav', 'const', 'image', 'man', 'maths', 'son','platformer_2d']