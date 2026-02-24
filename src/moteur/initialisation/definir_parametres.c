/* definit les parametres de la fenetre et de la boucle */
#include "../../main.h"
#include "../../prediction_branche.h"
#include "../../proprietes.h"
#include "../boucle/boucle.h"
#include "../fenetre/fenetre.h"
#include "../logging/logging.h"
#include "initialisation.h"
#include "mapping_data.h"

/* definit la taille de la fentre de lunivers */
void definir_taille(int largeur, int hauteur) {
    if (UNLIKELY(!gs))
        return;
    if (UNLIKELY(largeur <= 0 || hauteur <= 0)) {
        log_fmt(NiveauLogErreur, "invalid dimensions : %sx%s", largeur, hauteur);
        largeur = DEFAULT_WIDTH;
        hauteur = DEFAULT_HEIGHT;
    }
    gs->fenetre->largeur_univers = largeur;
    gs->fenetre->hauteur_univers = hauteur;
    if (gs->fenetre->fenetre) {
        SDL_SetWindowSize(gs->fenetre->fenetre, largeur, hauteur);
        SDL_SetWindowPosition(gs->fenetre->fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

void definir_fps(float fps) {
    if (UNLIKELY(!gs))
        return;
    if (UNLIKELY(fps <= 0.0f)) {
        log_fmt(NiveauLogErreur, "put invalid fps :%s", fps);
        fps = DEFAULT_FPS;
    }
    gs->timing->fps = fps;
}

void definir_titre(const char *titre) {
    if (UNLIKELY(!gs))
        return;
    if (UNLIKELY(!titre)) {
        log_message(NiveauLogErreur, "put invalid title");
        titre = DEFAULT_WINDOW_TITLE;
    }
    gs->fenetre->nom_fenetre = titre;
    SDL_SetWindowTitle(gs->fenetre->fenetre, titre);
}

void definir_bandes_noires(bool bande_noir) {
    if (UNLIKELY(!gs))
        return;
    gs->frame->fond->bande_noir = bande_noir;
}