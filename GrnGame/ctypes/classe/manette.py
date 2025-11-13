from ..structures import *
from ctypes import CDLL

class manette:
    def __init__(self, lib: CDLL, utils):
        self.jeu = lib
        self.utils = utils
        self._callback_ref = None
        self._user_update = None
    
    @property
    def _g(self):
        return self.utils.renvoie_gestionnaire()

    def enfoncee(self, touche_nom):
        g = self._g
        if not g:
            return False
        return self.jeu.touche_mannette_enfoncee(g, touche_nom.encode("utf-8"))

    def juste_presse(self, touche_nom):
        g = self._g
        if not g:
            return False
        return self.jeu.touche_mannette_juste_presse(g, touche_nom.encode("utf-8"))
    
    def init(self, index=0):
        g = self._g
        if not g:
            raise RuntimeError("Jeu non initialisé")
        return self.jeu.init_controller_joysticks(g, index)

    def renvoie_joysticks(self, dead_zone=0.1):
        g = self._g
        if not g:
            raise RuntimeError("Jeu non initialisé")
        entrees_ptr = g.contents.entrees
        if not entrees_ptr:
            return None

        ptr = self.jeu.renvoie_joysticks(entrees_ptr, dead_zone)
        if not ptr:
            return None
        return [ptr[i] for i in range(6)]

    def fermer(self):
        g = self._g
        if not g:
            return
        self.jeu.fermer_controller(g)
        self.jeu.fermer_joystick(g)

