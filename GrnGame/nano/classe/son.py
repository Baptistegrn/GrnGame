from ...chemin import renvoie_chemin_abs

class _son:
    def __init__(self, utils):
        self.utils = utils
        self.jeu = self.utils.jeu
    #francais
    def jouer(self, lien, boucle=0, canal=-1,volume = 64):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.jouer_son(g, lien_abs, boucle, canal,volume)

    def arreter(self, lien):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.arreter_son(g, lien_abs)

    def arreter_canal(self, canal):
        self.jeu.arreter_canal(canal)

    def pause_canal(self, canal):
        self.jeu.pause_canal(canal)

    def pause(self, lien):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.pause_son(g, lien_abs)

    def reprendre_canal(self, canal):
        self.jeu.reprendre_canal(canal)

    def reprendre(self, lien):
        g = self.utils._g
        if not g:
            return
        lien_abs = renvoie_chemin_abs(lien)
        self.jeu.reprendre_son(g, lien_abs)

    #anglais
    def play(self, path, loop=0, channel=-1, volume=64):
        return self.jouer(path, loop, channel, volume)

    def stop(self, path):
        return self.arreter(path)

    def stop_channel(self, channel):
        return self.arreter_canal(channel)

    def pause_channel(self, channel):
        return self.pause_canal(channel)

    def pause_sound(self, path):
        return self.pause(path)

    def resume_channel(self, channel):
        return self.reprendre_canal(channel)

    def resume(self, path):
        return self.reprendre(path)