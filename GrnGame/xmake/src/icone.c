#include "main.h"

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void Seticone(Gestionnaire *gs,const char *chemin) {
    SDL_Window *window = gs->fenetre; 
    SDL_Surface *icon = IMG_Load(chemin);

    if (icon == NULL) {
        fprintf(stderr, "Erreur chargement icone: %s\n", IMG_GetError());
        return;
    }
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
    return;
}
