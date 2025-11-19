import os
from .chemin import retour_tout_chemin
from .utilitaires.systeme import renvoie_systeme
from .chargerlib import charger_module_nanobind
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
        jeu = charger_module_nanobind("LibGrnGame",chemin_lib, os.path.join(chemin_package, "xmake"))

    except Exception:
        print("[!] Erreur lors du chargement de la lib, le moteur est désactivé.")

        
if jeu is not None and called_program not in COMMANDES_INTERDITES:
    from .nano.classe.utilitaires import utilitaires
    g = None
    utils = utilitaires(jeu)

    from .nano.classe.clavier import _clavier
    clavier = _clavier(utils)
    keyboard = clavier
    from .nano.classe.image import _image
    image = _image(utils)
    from .nano.classe.constantes import constante
    const = constante(utils)
    from .nano.classe.manette import _manette
    manette = _manette(utils)
    controller = manette
    from .nano.classe.son import _son
    son = _son(utils)
    song = son
    
    #symboles
    __all__ = ['utils', 'clavier','keyboard', 'const', 'image', 'manette','controller', 'son','song','platformer_2d']