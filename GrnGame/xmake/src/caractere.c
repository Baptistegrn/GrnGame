#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float ajouter_char_dans_tableau(Gestionnaire* jeu,  char* lien_image, char lettre, float posx, float posy,
                                float coeff, int sens, int rotation,int alpha)
{
    if (!jeu || !lien_image)
    {
        fprintf(stderr, "Erreur: Paramètre NULL lors de l'ajout d'un caractère\n");
        return 0.0f;
    }

    char lien_image_lettre[TAILLE_LIEN_GT];
    snprintf(lien_image_lettre, TAILLE_LIEN_GT, "%s/%d.png", lien_image, (unsigned char)lettre);
    normaliser_chemin(lien_image_lettre);
    SDL_Texture* texture = recuperer_texture_par_lien(jeu->textures, lien_image_lettre);
    if (!texture)
    {
        fprintf(stderr, "Erreur: Texture introuvable pour le caractère '%c' (code: %d)\n", lettre,
                (unsigned char)lettre);
        return 0.0f;
    }

    int texW = 0, texH = 0;
    if (SDL_QueryTexture(texture, NULL, NULL, &texW, &texH) != 0)
    {
        fprintf(stderr, "Erreur: Impossible d'obtenir les dimensions de la texture '%s': %s\n", lien_image_lettre,
                SDL_GetError());
        return 0.0f;
    }

    float largeur_finale = (float)texW * coeff;
    float hauteur_finale = (float)texH * coeff;

    ajouter_image_au_tableau(jeu, lien_image_lettre, posx, posy, largeur_finale, hauteur_finale, sens, rotation,alpha);

    return largeur_finale;
}

void ajouter_mot_dans_tableau(Gestionnaire* jeu, char* chemin, const char* mot, float posx, float posy,
                              float coeff, int sens, float ecart, int rotation,int alpha)
{
    if (!jeu || !chemin || !mot)
    {
        fprintf(stderr, "Erreur: Paramètre NULL lors de l'ajout d'un mot\n");
        return;
    }

    int taille_chaine = (int)strlen(mot);
    float position_courante = 0.0f;

    for (int i = 0; i < taille_chaine; i++)
    {
        float largeur =
            ajouter_char_dans_tableau(jeu, chemin, mot[i], posx + position_courante, posy, coeff, sens, rotation,alpha);
        position_courante += largeur + ecart;
    }
}

void ecrire_dans_console(const char* mot)
{
    if (!mot)
    {
        fprintf(stderr, "Erreur: Tentative d'écriture d'une chaîne NULL dans la console\n");
        return;
    }

    fprintf(stderr,"%s",mot);
}
