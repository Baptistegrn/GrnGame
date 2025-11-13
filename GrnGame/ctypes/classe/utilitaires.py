
from ..structures import *
from ...chemin import renvoie_chemin_abs,retour_tout_chemin
import sys
import os
from ctypes import CDLL
class utilitaires:
    def __init__(self, lib: CDLL):
        self.jeu = lib
        self._g = None
        self._callback_ref = None
        self._user_update = None
    def renvoie_gestionnaire(self):
        return self._g

    def init(self, largeur=160, hauteur=90, fps=60, coeff=3,
            chemin_image=".", chemin_son=".",
             bande_noir=True,
            update_func=None, nom_fenetre="fenetre",chemin_erreur ="erreurs.log"):
        
        chemin_erreur_abs = renvoie_chemin_abs(chemin_erreur)
        chemin_image_abs = renvoie_chemin_abs(chemin_image)
        chemin_son_abs = renvoie_chemin_abs(chemin_son)

        self._g = self.jeu.initialisation(
            hauteur, largeur, fps, coeff,
            chemin_image_abs.encode("utf-8"), chemin_son_abs.encode("utf-8"),
             bande_noir, nom_fenetre.encode("utf-8"),chemin_erreur_abs.encode("utf-8")
        )
        if not self._g:
            raise RuntimeError("Initialisation échouée")
        if getattr(sys, 'frozen', False):
            pass
        else:
            chemin_package, b, c = retour_tout_chemin()

            icone = os.path.join(chemin_package,"xmake", "icone.png")

            self.jeu.Seticone(self._g, icone.encode("utf-8"))


        if update_func:
            if not callable(update_func):
                raise ValueError("update_func doit être callable")
            self._user_update = update_func

            def wrapper(g):
                if self._user_update:
                    self._user_update()
            
            self._callback_ref = UpdateCallbackType(wrapper)
            self.jeu.set_update_callback(self._callback_ref)

        self.jeu.boucle_principale(self._g)

    def colorier(self, r, g, b):
        if not self._g:
            return
        return self.jeu.colorier(self._g, r, g, b)

    def redimensionner_fenetre(self):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        self.jeu.redimensionner_fenetre(self._g)

    def ecrire_console(self, mot):
        return self.jeu.ecrire_dans_console(mot.encode("utf-8"))

    def stopper_jeu(self):
        if self._g:
            self._g.contents.run = False


    def set_update_callback(self, py_func):
        if not callable(py_func):
            raise ValueError("update doit être une fonction")
        self._user_update = py_func

    def update(self):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        self.jeu.update(self._g)
        if self._user_update:
            self._user_update()