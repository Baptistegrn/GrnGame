class constante:
    def __init__(self, utils):
        self.utils = utils
        self.jeu = utils.jeu
    # francais
    @property
    def largeur(self):
        g = self.utils._g
        if not g:
            return 0
        return g.largeur
    
    @property
    def hauteur(self):
        g = self.utils._g
        if not g:
            return 0
        return g.hauteur
    
    @property
    def dt(self):
        g = self.utils._g
        if not g:
            return 0.0
        return g.dt
    
    @property
    def fps(self):
        g = self.utils._g
        if not g:
            return 0.0
        return g.fps
    #time est francais et anglais
    @property
    def time(self):
        g = self.utils._g
        if not g:
            return 0
        return g.temps_frame
    
    @property
    def souris_x(self):
        g = self.utils._g
        if not g:
            return 0
        return g.entrees.mouse_x
    
    @property
    def souris_y(self):
        g = self.utils._g
        if not g:
            return 0
        return g.entrees.mouse_y
    
    @property
    def mouse_presse(self):
        g = self.utils._g
        if not g:
            return False
        return g.entrees.mouse_pressed
    
    @property
    def mouse_juste_presse(self):
        g = self.utils._g
        if not g:
            return False
        return g.entrees.mouse_just_pressed
    
    @property
    def mouse_droit_presse(self):
        g = self.utils._g
        if not g:
            return False
        return g.entrees.mouse_right_pressed
    
    @property
    def mouse_droit_juste_presse(self):
        g = self.utils._g
        if not g:
            return False
        return g.entrees.mouse_right_just_pressed
    
    @property
    def decalage_x(self):
        g = self.utils._g
        if not g:
            return 0
        return g.decalage_x / (g.largeur_actuel / g.largeur)
    
    @property
    def decalage_y(self):
        g = self.utils._g
        if not g:
            return 0
        return g.decalage_y / (g.hauteur_actuel / g.hauteur)
    
    @property
    def en_marche(self):
        g = self.utils._g
        if not g:
            return False
        return g.run
    
    #anglais
    @property
    def width(self):
        return self.largeur

    @property
    def height(self):
        return self.hauteur

    @property
    def mouse_x(self):
        return self.souris_x

    @property
    def mouse_y(self):
        return self.souris_y

    @property
    def mouse_pressed(self):
        return self.mouse_presse

    @property
    def mouse_just_pressed(self):
        return self.mouse_juste_presse

    @property
    def mouse_right_pressed(self):
        return self.mouse_droit_presse

    @property
    def mouse_right_just_pressed(self):
        return self.mouse_droit_juste_presse

    @property
    def offset_x(self):
        return self.decalage_x

    @property
    def offset_y(self):
        return self.decalage_y

    @property
    def running(self):
        return self.en_marche