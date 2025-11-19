from ...chemin import renvoie_chemin_abs, retour_tout_chemin
import sys
import os

class utilitaires:
    def __init__(self, lib):
        self.jeu = lib
        self._g = None
        self._user_update = None
    #francais

    def init(self, largeur=160, hauteur=90, fps=60, coeff=3,
             chemin_image=".", chemin_son=".",
             bande_noir=True, update_func=None, nom_fenetre="fenetre",
             chemin_erreur="erreurs.log"):

        chemin_erreur_abs = renvoie_chemin_abs(chemin_erreur)
        chemin_image_abs = renvoie_chemin_abs(chemin_image)
        chemin_son_abs = renvoie_chemin_abs(chemin_son)

        self._g = self.jeu.initialisation(
            hauteur, largeur, fps, coeff,
            chemin_image_abs, chemin_son_abs,
            bande_noir, nom_fenetre, chemin_erreur_abs
        )

        if not self._g:
            raise RuntimeError("Initialisation échouée")

        # icone si pas compile
        if not getattr(sys, 'frozen', False):
            chemin_package, _, _ = retour_tout_chemin()
            icone = os.path.join(chemin_package, "icone", "icone.png")
            self.jeu.Seticone(self._g, icone)

        if update_func:
            #erreur
            if not callable(update_func):
                raise ValueError("update_func doit être callable")
            self._user_update = update_func

            def wrapper(g):
                if self._user_update:
                    self._user_update()

            self.jeu.set_update_callback(wrapper)
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
        return self.jeu.ecrire_console(mot)

    def stopper_jeu(self):
        if self._g:
            self._g.run = False

    def set_update_callback(self, py_func):
        if not callable(py_func):
            raise ValueError("update doit être une fonction")
        self._user_update = py_func
        self.jeu.set_update_callback(py_func)

    def update(self):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        self.jeu.update(self._g)
        if self._user_update:
            self._user_update()

    #anglais
    
    def initialize(self, width=160, height=90, fps=60, scale=3,
                   image_path=".", sound_path=".",
                   letterbox=True, update_func=None, window_name="window",
                   error_path="errors.log"):
        return self.init(width, height, fps, scale,
                        image_path, sound_path,
                        letterbox, update_func, window_name,
                        error_path)
    
    def colorize(self, r, g, b):
        return self.colorier(r, g, b)
    
    def resize_window(self):
        return self.redimensionner_fenetre()
    
    def write_console(self, text):
        return self.ecrire_console(text)
    
    def stop_game(self):
        return self.stopper_jeu()
    

    def platformer_2d(self, dt, pos_x, pos_y, vitesse_y, en_air,
                      larg_joueur, haut_joueur, blocs,
                      gravite=400.0, force_saut=-200.0,
                      vitesse_max_chute=500.0, correction_mur=100.0,
                      activer_saut=False):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        
        return self.jeu.platformer_2d(
            float(dt), float(pos_x), float(pos_y), float(vitesse_y), en_air,
            float(larg_joueur), float(haut_joueur), blocs,
            float(gravite), float(force_saut), float(vitesse_max_chute), float(correction_mur),
            activer_saut
        )