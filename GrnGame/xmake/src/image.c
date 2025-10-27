#include "main.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ajouter_image_au_jeu(Gestionnaire* gestionnaire, image nouvelle)
{
    if (!gestionnaire || !gestionnaire->image)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou tableau d'images NULL\n");
        return;
    }

    Tableau_image* jeu = gestionnaire->image;

    if (jeu->nb_images >= jeu->capacite_images)
    {
        int nouvelle_capacite = (jeu->capacite_images == 0) ? 100 : jeu->capacite_images + 50;
        image* nouveau_tab = realloc(jeu->tab, sizeof(image) * nouvelle_capacite);

        if (!nouveau_tab)
        {
            fprintf(stderr, "Erreur: Échec de réallocation mémoire pour les images (capacité: %d -> %d)\n",
                    jeu->capacite_images, nouvelle_capacite);
            return;
        }

        jeu->tab = nouveau_tab;
        jeu->capacite_images = nouvelle_capacite;
    }

    jeu->tab[jeu->nb_images++] = nouvelle;
}

void ajouter_image_au_tableau(Gestionnaire* gestionnaire, const char* id, float x, float y, float w, float h, int sens,
                              int rotation)
{
    if (!gestionnaire)
    {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans ajouter_image_au_tableau()\n");
        return;
    }

    if (!gestionnaire->textures)
    {
        fprintf(stderr, "Erreur: Gestionnaire de textures non initialisé\n");
        return;
    }

    if (!id)
    {
        fprintf(stderr, "Erreur: ID de texture NULL\n");
        return;
    }

    image img;
    memset(&img, 0, sizeof(image));
    img.posx = x;
    img.posy = y;
    img.taillex = w;
    img.tailley = h;
    img.sens = sens;
    img.rotation = rotation;

    SDL_Texture* tex = recuperer_texture_par_lien(gestionnaire->textures, id);
    if (!tex)
    {
        fprintf(stderr, "Erreur: Texture introuvable '%s'\n", id);
        return;
    }

    img.texture = tex;
    ajouter_image_au_jeu(gestionnaire, img);
}

void ajouter_image_au_tableau_batch(Gestionnaire* gestionnaire, const char** id, float* x, float* y, float* w, float* h,
                                    int* sens, int* rotation, int taille)
{
    if (!gestionnaire)
    {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans ajouter_image_au_tableau_batch()\n");
        return;
    }

    if (!id || !x || !y || !w || !h || !sens || !rotation)
    {
        fprintf(stderr, "Erreur: Paramètres NULL dans ajouter_image_au_tableau_batch()\n");
        return;
    }

    if (taille <= 0)
    {
        fprintf(stderr, "Erreur: Taille de batch invalide (%d)\n", taille);
        return;
    }

    for (int i = 0; i < taille; i++)
    {
        ajouter_image_au_tableau(gestionnaire, id[i], x[i], y[i], w[i], h[i], sens[i], rotation[i]);
    }
}

void afficher_images(Gestionnaire* gestionnaire)
{
    if (!gestionnaire || !gestionnaire->rendu || !gestionnaire->image)
    {
        fprintf(stderr, "Erreur: Composants manquants pour afficher les images\n");
        return;
    }

    Tableau_image* jeu = gestionnaire->image;
    float coeff_largeur = (float)gestionnaire->largeur_actuel / (float)gestionnaire->largeur;
    float coeff_hauteur = (float)gestionnaire->hauteur_actuel / (float)gestionnaire->hauteur;

    for (int i = 0; i < jeu->nb_images; i++)
    {
        image* img = &jeu->tab[i];

        if (!img->texture)
        {
            continue;
        }
        if (img->posx > gestionnaire->largeur || img->posx < -img->taillex || img->posy > gestionnaire->hauteur ||
            img->posy < -img->tailley)
        {
            continue;
        }

        SDL_Rect dst = {(int)lroundf(img->posx * coeff_largeur + gestionnaire->decalage_x),
                        (int)lroundf(img->posy * coeff_hauteur + gestionnaire->decalage_y),
                        (int)lroundf(img->taillex * coeff_largeur), (int)lroundf(img->tailley * coeff_hauteur)};

        SDL_Point centre = {dst.w / 2, dst.h / 2};
        SDL_RendererFlip flip = (img->sens == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        if (SDL_RenderCopyEx(gestionnaire->rendu, img->texture, NULL, &dst, img->rotation, &centre, flip) != 0)
        {
            fprintf(stderr, "Erreur: Échec de rendu de l'image %d: %s\n", i, SDL_GetError());
        }
    }

    jeu->nb_images = 0;
}

void dessiner_bandes_noires(SDL_Renderer* rendu, double decalage_x, double decalage_y, int largeur, int hauteur)
{
    if (!rendu)
    {
        fprintf(stderr, "Erreur: Renderer NULL dans dessiner_bandes_noires()\n");
        return;
    }

    SDL_SetRenderDrawColor(rendu, 10, 10, 10, 255);

    int dx = (int)lround(decalage_x);
    int dy = (int)lround(decalage_y);


    SDL_Rect rect_gauche = {0, 0, dx, hauteur};
    SDL_RenderFillRect(rendu, &rect_gauche);

    SDL_Rect rect_droite = {largeur - dx, 0, dx, hauteur};
    SDL_RenderFillRect(rendu, &rect_droite);


    SDL_Rect rect_haut = {0, 0, largeur, dy};
    SDL_RenderFillRect(rendu, &rect_haut);

    SDL_Rect rect_bas = {0, hauteur - dy, largeur, dy};
    SDL_RenderFillRect(rendu, &rect_bas);
}

void actualiser(Gestionnaire* jeu, bool colorier, bool bande_noir, int r, int g, int b)
{
    if (!jeu || !jeu->rendu)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou renderer NULL dans actualiser()\n");
        return;
    }

    SDL_Renderer* rendu = jeu->rendu;

    if (colorier)
    {
        SDL_SetRenderDrawColor(rendu, r, g, b, 255);
    }
    SDL_RenderClear(rendu);

    afficher_images(jeu);

    if (bande_noir)
    {
        int largeur, hauteur;
        SDL_GetWindowSize(jeu->fenetre, &largeur, &hauteur);
        dessiner_bandes_noires(rendu, jeu->decalage_x, jeu->decalage_y, largeur, hauteur);
    }

    SDL_RenderPresent(rendu);
}
