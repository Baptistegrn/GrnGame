class _clavier:
    # francais
    def __init__(self, utils):
        self.jeu = utils.jeu
        self.utils = utils 

    def juste_presse(self, touche_nom):
        g = self.utils._g
        if g is None:
            return False
        return self.jeu.touche_juste_presse(g, touche_nom)

    def enfoncee(self, touche_nom):
        g = self.utils._g
        if g is None:
            return False
        return self.jeu.touche_enfoncee(g, touche_nom)
    
    # anglais
    def just_pressed(self, touch_name):
        g = self.utils._g
        if g is None:
            return False
        return self.jeu.touche_juste_presse(g, touch_name)

    def pressed(self, touch_name):
        g = self.utils._g
        if g is None:
            return False
        return self.jeu.touche_enfoncee(g, touch_name)
