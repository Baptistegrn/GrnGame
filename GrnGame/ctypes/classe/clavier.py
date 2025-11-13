from ..structures import *
from .utilitaires import utilitaires
from ctypes import CDLL

class clavier:
    def __init__(self, lib: CDLL, utils):
        self.jeu = lib
        self.utils = utils
    
    @property
    def _g(self):
        return self.utils.renvoie_gestionnaire()

    def juste_presser(self, touche_nom):
        g = self._g  
        if not g:
            return False
        return self.jeu.touche_juste_presse(g, touche_nom.encode("utf-8"))

    def enfoncee(self, touche_nom):
        g = self._g
        if not g:
            return False
        return self.jeu.touche_enfoncee(g, touche_nom.encode("utf-8"))