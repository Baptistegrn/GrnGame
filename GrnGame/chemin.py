import os
import sys
import platform
from .utilitaires.renvoielib import renvoie_lib
from .utilitaires.systeme import renvoie_systeme

def retour_tout_chemin():
    libname = renvoie_lib()

    chemin_package = os.path.dirname(os.path.abspath(__file__))

    if getattr(sys, 'frozen', False):
        base = os.path.dirname(sys.executable)
        
        temp = getattr(sys, '_MEIPASS', base)

        chemin_script = base
        chemin_lib = os.path.join(temp, libname)

    else:
        systeme = renvoie_systeme() 
        ver_majeur = sys.version_info.major
        ver_mineur = sys.version_info.minor
        dossier_python = f"python{ver_majeur}.{ver_mineur}"
        chemin_lib = os.path.join(chemin_package, "dist", systeme, dossier_python, libname)
        chemin_script = os.path.dirname(os.path.abspath(sys.argv[0]))

    return chemin_package, chemin_script, chemin_lib

def renvoie_chemin_abs(relative_path):
    x,chemin_script,y = retour_tout_chemin()
    if os.path.isabs(relative_path):
        return relative_path
    else:
        return os.path.join(chemin_script, relative_path)
