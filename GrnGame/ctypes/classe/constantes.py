from ..structures import *
from ctypes import CDLL

class constante:
    def __init__(self, lib: CDLL, utils):
        self.jeu = lib
        self.utils = utils
        self._callback_ref = None
        self._user_update = None
    
    @property
    def _g(self):
        return self.utils.renvoie_gestionnaire()
    
    @property
    def largeur(self):
        g = self._g  
        return g.contents.largeur if g else 0
    
    @property
    def hauteur(self):
        g = self._g  
        return g.contents.hauteur if g else 0
    
    @property
    def dt(self):
        g = self._g  
        return g.contents.dt if g else 0.0
    
    @property
    def fps(self):
        g = self._g 
        return g.contents.fps if g else 0.0
    
    @property
    def time(self):
        g = self._g 
        return g.contents.temps_frame if g else 0
    
    @property
    def mouse_x(self):
        g = self._g 
        return g.contents.entrees.contents.mouse_x if g else 0
    
    @property
    def mouse_y(self):
        g = self._g 
        return g.contents.entrees.contents.mouse_y if g else 0
    
    @property
    def mouse_presse(self):
        g = self._g 
        return g.contents.entrees.contents.mouse_pressed if g else False
    
    @property
    def mouse_juste_presse(self):
        g = self._g  
        return g.contents.entrees.contents.mouse_just_pressed if g else False
    
    @property
    def mouse_droit_presse(self):
        g = self._g  
        return g.contents.entrees.contents.mouse_right_pressed if g else False
    
    @property
    def mouse_droit_juste_presse(self):
        g = self._g  
        return g.contents.entrees.contents.mouse_right_just_pressed if g else False
    
    @property
    def decalage_x(self):
        g = self._g 
        if not g:
            return 0
        return g.contents.decalage_x / (
            g.contents.largeur_actuel / g.contents.largeur
        )
    
    @property
    def decalage_y(self):
        g = self._g  
        if not g:
            return 0
        return g.contents.decalage_y / (
            g.contents.hauteur_actuel / g.contents.hauteur
        )
    
    @property
    def run(self):
        g = self._g 
        return g.contents.run if g else False