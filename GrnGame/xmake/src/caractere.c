#include "logging.h"
#include "main.h"

#include <stdio.h>
#include <string.h>

float ajouter_char_dans_tableau(Gestionnaire* jeu,  char* lien_image, char lettre, float posx, float posy,
                                float coeff, int sens, int rotation,int alpha)
{
    if (!jeu || !lien_image)
    {
        log_fmt(NiveauLogErreur, "Erreur: Paramètre NULL lors de l'ajout d'un caractère\n");
        return 0.0f;
    }

    char lien_image_lettre[TAILLE_LIEN_GT];
    snprintf(lien_image_lettre, TAILLE_LIEN_GT, "%s/%d.png", lien_image, (unsigned char)lettre);
    normaliser_chemin(lien_image_lettre);
    SDL_Texture* texture = recuperer_texture_par_lien(jeu->textures, lien_image_lettre);
    if (!texture)
    {
        log_fmt(NiveauLogAvertissement, "Erreur: Texture introuvable pour le caractère '%c' (code: %d)\n", lettre,
                (unsigned char)lettre);
        return 0.0f;
    }

    int texW = 0, texH = 0;
    if (SDL_QueryTexture(texture, NULL, NULL, &texW, &texH) != 0)
    {
        log_fmt(NiveauLogAvertissement, "Erreur: Impossible d'obtenir les dimensions de la texture '%s': %s\n", lien_image_lettre,
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
        log_fmt(NiveauLogErreur, "Erreur: Paramètre NULL lors de l'ajout d'un mot\n");
        return;
    }
    #include <math.h>
    int taille_chaine = (int)strlen(mot);
    float position_courante = 0.0f;

    double decalage_x_scaled = (double)jeu->decalage_x / ( (double)jeu->largeur_actuel / (double)jeu->largeur);
    double decalage_y_scaled = (double)jeu->decalage_y / ( (double)jeu->largeur_actuel / (double)jeu->largeur);
    double coeff_d =    (double)jeu->largeur_actuel / (double)jeu->largeur;
    double posx_d = (double)posx;
    double posy_d = (double)posy;
    posx = (float)(lround(((posx_d ) * coeff_d) + decalage_x_scaled)/ coeff_d);
    posy = (float)(lround(((posy_d ) * coeff_d)+ decalage_y_scaled) / coeff_d);
    for (int i = 0; i < taille_chaine; i++)
    {
        float largeur =
            ajouter_char_dans_tableau(jeu, chemin, mot[i], posx + position_courante, posy, coeff, sens, rotation,alpha);
        position_courante += largeur + ecart;
    }
}

