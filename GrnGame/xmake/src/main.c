#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[])
{
    int largeur = 300;
    int hauteur = 300;
    char *lien = "./assets";
    Gestionnaire *jeu = initialisation(hauteur, largeur, 200.0f, 3, lien, lien,  false, "coucou","err.log");
    boucle_principale(jeu);
    return EXIT_SUCCESS;
}
