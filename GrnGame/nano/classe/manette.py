class _manette:
    def __init__(self, utils):
        self.utils = utils
        self.jeu = self.utils.jeu

    #francais
    
    def enfoncee(self, touche_nom):
        g = self.utils._g
        if not g:
            return False
        return self.jeu.touche_mannette_enfoncee(g, touche_nom)

    def juste_presse(self, touche_nom):
        g = self.utils._g
        if not g:
            return False
        return self.jeu.touche_mannette_juste_presse(g, touche_nom)
    
    def init(self, index=0):
        g = self.utils._g
        if not g:
            return None
        return self.jeu.init_controller_joysticks(g, index)

    def renvoie_joysticks(self, dead_zone=0.1):
        g = self.utils._g
        if not g:
            return [0.0 for i in range(6)]
        ptr = self.jeu.renvoie_joysticks(g, dead_zone)
        if not ptr:
            return [0.0 for i in range(6)]
        
        return [ptr[i] for i in range(6)]

    def fermer(self):
        g = self.utils._g
        if not g:
            return
        self.jeu.fermer_controller_joystick(g)

    #anglais
    
    def pressed(self, button_name):
        return self.enfoncee(button_name)
    
    def just_pressed(self, button_name):
        return self.juste_presse(button_name)
    
    def initialize(self, index=0):
        return self.init(index)
    
    def get_joysticks(self, dead_zone=0.1):
        return self.renvoie_joysticks(dead_zone)
    
    def close(self):
        return self.fermer()