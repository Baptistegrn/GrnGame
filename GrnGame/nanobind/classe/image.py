from ...chemin import renvoie_chemin_abs

class _image:
    def __init__(self, utils):
        self.utils = utils
        self.jeu = utils.jeu

    #francais
 
    def dessiner(self, lien, x, y, w, h, sens=0, rotation=0, transparence=255):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        return self.jeu.ajouter_image(
            g, lien_abs,int(x), int(y), int(w), int(h), sens, rotation, transparence
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
        xs = [int(v) for v in xs]
        ys = [int(v) for v in ys]
        ws = [int(v) for v in ws]
        hs = [int(v) for v in hs]
        for i in range(taille):
            ids[i] = renvoie_chemin_abs(ids[i])

        return self.jeu.ajouter_image_batch(
            g, ids, xs, ys, ws, hs, sens, rotations, transparence
        )
    def dessiner_ligne(self, x1, y1, x2, y2, r=255, g=255, b=255):
            ges = self.utils._g
            if not ges:
                return
            return self.jeu.ajouter_ligne(
                ges, 
                int(x1), int(y1), 
                int(x2), int(y2), 
                int(r), int(g), int(b)
            )

    def dessiner_rectangle(self, x, y, w, h, r=255, g=255, b=255, plein=True):
        ges = self.utils._g
        if not ges:
            return
        return self.jeu.ajouter_rectangle(
            ges, 
            int(x), int(y), 
            int(w), int(h), 
            int(r), int(g), int(b), 
            int(plein)
        )

    def dessiner_cercle(self, x, y, rayon, r=255, g=255, b=255, plein=True):
        ges = self.utils._g
        if not ges:
            return
        return self.jeu.ajouter_cercle(
            ges, 
            int(x), int(y), 
            int(rayon), 
            int(r), int(g), int(b), 
            int(plein)
        )

    def dessiner_triangle(self, x1, y1, x2, y2, r=255, g=255, b=255, plein=True):
        ges = self.utils._g
        if not ges:
            return
        return self.jeu.ajouter_triangle(
            ges, 
            int(x1), int(y1), 
            int(x2), int(y2), 
            int(r), int(g), int(b), 
            int(plein))
        
    def dessiner_mot(self, lien, mot, x, y, coeff, ecart, sens=0, rotation=0, alpha=255):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        return self.jeu.ajouter_mot(
            g, lien_abs, mot,
            int(x), int(y), int(coeff), sens, int(ecart), rotation, alpha
        )
        
    def appliquer_shaders(self, xs, ys, rs=None, gs=None, bs=None, transparences=None):
        ges = self.utils._g
        if not ges:
            return
        taille = len(xs)
        if rs is None:
            rs = [155] * taille
        if gs is None:
            gs = [155] * taille
        if bs is None:
            bs = [20] * taille
        if transparences is None:
            transparences = [125] * taille
        xs = [int(v) for v in xs]
        ys = [int(v) for v in ys]

        return self.jeu.appliquer_shaders(
            ges,
            xs,
            ys,
            rs,
            gs,
            bs,
            transparences
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
    
    def draw(self, path, x, y, w, h, direction=0, rotation=0, transparency=255):
        return self.dessiner(path, x, y, w, h, direction, rotation, transparency)
    
    def draw_batch(self, ids, xs, ys, ws, hs, directions=None, rotations=None, transparency=None):
        return self.dessiner_batch(ids, xs, ys, ws, hs, directions, rotations, transparency)
    
    def draw_word(self, path, word, x, y, scale, spacing, direction=0, rotation=0, alpha=255):
        return self.dessiner_mot(path, word, x, y, scale, spacing, direction, rotation, alpha)
    
    def apply_shaders(self, xs, ys, rs=None, gs=None, bs=None, alphas=None):
        return self.appliquer_shaders(xs, ys, rs, gs, bs, alphas)
    
    def load_all_sounds(self, folder):
        return self.charger_tous_les_sons(folder)
    
    def load_all_textures(self, folder):
        return self.charger_toutes_les_textures(folder)
    
    def free_sound_manager(self):
        return self.liberer_gestionnaire_son()
    
    def free_image_manager(self):
        return self.liberer_gestionnaire_image()
    
    def draw_rectangle(self, x, y, w, h, r=255, g=255, b=255, filled=True):
        return self.dessiner_rectangle(x, y, w, h, r, g, b, filled)
    
    def draw_triangle(self, x1, y1, x2, y2, r=255, g=255, b=255, filled=True):
        return self.dessiner_triangle(x1, y1, x2, y2, r, g, b, filled)
    
    def draw_circle(self, x, y, radius, r=255, g=255, b=255, filled=True):
        return self.dessiner_cercle(x, y, radius, r, g, b, filled)
    
    def draw_line(self, x1, y1, x2, y2, r=255, g=255, b=255):
        return self.dessiner_ligne(x1, y1, x2, y2, r, g, b)
    