from ..structures import *
from ctypes import CDLL
from ...chemin import renvoie_chemin_abs

class _son:
    def __init__(self, lib: CDLL, utils):
        self.jeu = lib
        self.utils = utils
        self._callback_ref = None
        self._user_update = None
    
    @property
    def _g(self):
        return self.utils.renvoie_gestionnaire()

    def jouer(self, lien, boucle=0, canal=-1,volume = 64):
        g = self._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.jouer_son(g, lien_abs.encode("utf-8"), boucle, canal,volume)

    def arreter(self, lien):
        g = self._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.arreter_son(g, lien_abs.encode("utf-8"))

    def arreter_canal(self, canal):
        self.jeu.arreter_canal(canal)

    def pause_canal(self, canal):
        self.jeu.pause_canal(canal)

    def pause(self, lien):
        g = self._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.pause_son(g, lien_abs.encode("utf-8"))

    def reprendre_canal(self, canal):
        self.jeu.reprendre_canal(canal)

    def reprendre(self, lien):
        g = self._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.reprendre_son(g, lien_abs.encode("utf-8"))