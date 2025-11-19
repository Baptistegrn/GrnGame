from ...chemin import renvoie_chemin_abs

class _image:
    def __init__(self, utils):
        self.utils = utils
        self.jeu = utils.jeu
    
    #francais
    
    def dessiner_forme(self, x, y, w, h, sens=0, rotation=0, transparence=255, r=255, g=255, b=255):
        ges = self.utils._g
        if not ges:
            return
        return self.jeu.ajouter_forme(ges, float(x), float(y), float(w), float(h), sens, rotation, transparence, r, g, b)

    def dessiner_forme_batch(self, xs, ys, ws, hs, sens=None, rotations=None, transparences=None, rs=None, gs=None, bs=None):
        ges = self.utils._g
        if not ges:
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
        xs = [float(v) for v in xs]
        ys = [float(v) for v in ys]
        ws = [float(v) for v in ws]
        hs = [float(v) for v in hs]

        return self.jeu.ajouter_forme_batch(
            ges, xs, ys, ws, hs, sens, rotations, transparences, rs, gs, bs
        )

    def dessiner(self, lien, x, y, w, h, sens=0, rotation=0, transparence=255):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        return self.jeu.ajouter_image(
            g, lien_abs, float(x), float(y), float(w), float(h), sens, rotation, transparence
        )

    def dessiner_batch(self, ids, xs, ys, ws, hs, sens=None, rotations=None, transparence=None):
        g = self.utils._g
        if not g:
            return

        taille = len(xs)
        if sens is None:
            sens = [0] * taille
        if rotations is None:
            rotations = [0] * taille
        if transparence is None:
            transparence = [255] * taille
        xs = [float(v) for v in xs]
        ys = [float(v) for v in ys]
        ws = [float(v) for v in ws]
        hs = [float(v) for v in hs]
        for i in range(taille):
            ids[i] = renvoie_chemin_abs(ids[i])

        return self.jeu.ajouter_image_batch(
            g, ids, xs, ys, ws, hs, sens, rotations, transparence
        )
    
    def dessiner_mot(self, lien, mot, x, y, coeff, ecart, sens=0, rotation=0, alpha=255):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        return self.jeu.ajouter_mot(
            g, lien_abs, mot,
            float(x), float(y), float(coeff), sens, float(ecart), rotation, alpha
        )

    def charger_tous_les_sons(self, dossier):
        g = self.utils._g
        if not g:
            return
        dossier_abs = renvoie_chemin_abs(dossier)
        return self.jeu.charger_tous_les_sons(g, dossier_abs)

    def charger_toutes_les_textures(self, dossier):
        g = self.utils._g
        if not g:
            return
        dossier_abs = renvoie_chemin_abs(dossier)
        return self.jeu.charger_toutes_les_textures(g, dossier_abs)

    def liberer_gestionnaire_son(self):
        g = self.utils._g
        if not g:
            return
        return self.jeu.liberer_gestionnaire_son(g)

    def liberer_gestionnaire_image(self):
        g = self.utils._g
        if not g:
            return
        return self.jeu.liberer_gestionnaire_image(g)
    
    #anglais
    
    def draw_shape(self, x, y, w, h, direction=0, rotation=0, transparency=255, r=255, g=255, b=255):
        return self.dessiner_forme(x, y, w, h, direction, rotation, transparency, r, g, b)
    
    def draw_shape_batch(self, xs, ys, ws, hs, directions=None, rotations=None, transparencies=None, rs=None, gs=None, bs=None):
        return self.dessiner_forme_batch(xs, ys, ws, hs, directions, rotations, transparencies, rs, gs, bs)
    
    def draw(self, path, x, y, w, h, direction=0, rotation=0, transparency=255):
        return self.dessiner(path, x, y, w, h, direction, rotation, transparency)
    
    def draw_batch(self, ids, xs, ys, ws, hs, directions=None, rotations=None, transparency=None):
        return self.dessiner_batch(ids, xs, ys, ws, hs, directions, rotations, transparency)
    
    def draw_word(self, path, word, x, y, scale, spacing, direction=0, rotation=0, alpha=255):
        return self.dessiner_mot(path, word, x, y, scale, spacing, direction, rotation, alpha)
    
    def load_all_sounds(self, folder):
        return self.charger_tous_les_sons(folder)
    
    def load_all_textures(self, folder):
        return self.charger_toutes_les_textures(folder)
    
    def free_sound_manager(self):
        return self.liberer_gestionnaire_son()
    
    def free_image_manager(self):
        return self.liberer_gestionnaire_image()