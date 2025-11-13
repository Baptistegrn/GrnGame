from ..structures import *
from ctypes import CDLL, POINTER, c_float, c_int
from ...chemin import renvoie_chemin_abs

class _image:
    def __init__(self, lib: CDLL, utils):
        self.jeu = lib
        self.utils = utils 
        self._callback_ref = None
        self._user_update = None
    
    @property
    def _g(self):
        return self.utils.renvoie_gestionnaire()
    
    def dessiner_forme(self, x, y, w, h, sens=0, rotation=0, transparence=255, r=255, g=255, b=255):
        ges = self._g
        if not ges:
            return
        return self.jeu.ajouter_forme_au_tableau(
            ges,
            x, y,     
            w, h,    
            sens,
            rotation,
            transparence,
            r, g, b
        )

    def dessiner_forme_batch(self, xs, ys, ws, hs, sens=None, rotations=None, transparences=None, rs=None, gs=None, bs=None):
        g = self._g
        if not g:
            return
        
        taille = len(xs)
        if sens is None:
            sens = [0] * taille
        if rotations is None:
            rotations = [0] * taille
        if transparences is None:
            transparences = [255] * taille
        if rs is None:
            rs = [255] * taille
        if gs is None:
            gs = [255] * taille
        if bs is None:
            bs = [255] * taille
        xs_c = (c_float * taille)(*xs)
        ys_c = (c_float * taille)(*ys)
        ws_c = (c_float * taille)(*ws)
        hs_c = (c_float * taille)(*hs)
        sens_c = (c_int * taille)(*sens)
        rotations_c = (c_int * taille)(*rotations)
        transparences_c = (c_int * taille)(*transparences)
        rs_c = (c_int * taille)(*rs)
        gs_c = (c_int * taille)(*gs)
        bs_c = (c_int * taille)(*bs)
        self.jeu.ajouter_forme_au_tableau_batch(
            g, xs_c, ys_c, ws_c, hs_c,
            sens_c, rotations_c, transparences_c,
            rs_c, gs_c, bs_c, c_int(taille)
        )

    def dessiner(self, lien, x, y, w, h, sens=0, rotation=0, transparence=255):
        g = self._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        return self.jeu.ajouter_image_au_tableau(
            g, lien_abs.encode("utf-8"), x, y, w, h, sens, rotation, transparence
        )

    def dessiner_batch(self, ids, xs, ys, ws, hs, sens=None, rotations=None, transparence=None):
        g = self._g
        if not g:
            return
        
        taille = len(ids)
        if sens is None:
            sens = [0] * taille
        if rotations is None:
            rotations = [0] * taille

        ids_abs = [renvoie_chemin_abs(s) for s in ids]

        ids_c = (c_char_p * taille)(*(s.encode("utf-8") for s in ids_abs))
        xs_c = (c_float * taille)(*xs)
        ys_c = (c_float * taille)(*ys)
        ws_c = (c_float * taille)(*ws)
        hs_c = (c_float * taille)(*hs)
        sens_c = (c_int * taille)(*sens)
        rotations_c = (c_int * taille)(*rotations)
        transparence_c = (c_int * taille)(*transparence)
        self.jeu.ajouter_image_au_tableau_batch(
            g, ids_c, xs_c, ys_c, ws_c, hs_c,
            sens_c, rotations_c, transparence_c, c_int(taille)
        )

    def dessiner_mot(self, lien, mot, x, y, coeff, ecart, sens=0, rotation=0, alpha=255):
        g = self._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        return self.jeu.ajouter_mot_dans_tableau(
            g, lien_abs.encode("utf-8"), mot.encode("utf-8"),
            x, y, coeff, sens, ecart, rotation, alpha
        )