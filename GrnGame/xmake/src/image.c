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

void ajouter_image_au_tableau(Gestionnaire* gestionnaire, char* id, float x, float y, float w, float h, int sens,
                              int rotation,int transparence)
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


    image img;
    memset(&img, 0, sizeof(image));
    img.posx = x;
    img.posy = y;
    img.taillex = w;
    img.tailley = h;
    img.sens = sens;
    img.rotation = rotation;
    img.transparence = transparence;
    img.forme = false;

    SDL_Texture* tex = recuperer_texture_par_lien(gestionnaire->textures, id);
    if (!tex)
    {
        fprintf(stderr, "Erreur: Texture introuvable '%s'\n", id);
        return;
    }

    img.texture = tex;
    ajouter_image_au_jeu(gestionnaire, img);
}

void ajouter_forme_au_tableau(Gestionnaire* gestionnaire, float x, float y,
                                 float w, float h, int sens,
                              int rotation,int transparence,int r,int g, int b)
{
    if (!gestionnaire)
    {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans ajouter_forme_au_tableau()\n");
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
    img.transparence =transparence;
    img.forme = true;
    img.r =r;
    img.g =g;
    img.b = b;
    

    img.texture = NULL;
    ajouter_image_au_jeu(gestionnaire, img);
}





void ajouter_image_au_tableau_batch(Gestionnaire* gestionnaire, char** id, float* x, float* y, float* w, float* h,
                                    int* sens, int* rotation,int* transparence, int taille)
{
    if (!gestionnaire)
    {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans ajouter_image_au_tableau_batch()\n");
        return;
    }

    if (taille <= 0)
    {
        fprintf(stderr, "Erreur: Taille de batch invalide (%d)\n", taille);
        return;
    }

    for (int i = 0; i < taille; i++)
    {
        ajouter_image_au_tableau(gestionnaire, id[i], x[i], y[i], w[i], h[i], sens[i], rotation[i],transparence[i]);
    }
}


void ajouter_forme_au_tableau_batch(Gestionnaire* gestionnaire, float *x, float *y, float *w, float *h, int *sens,
                              int *rotation,int *transparence,int *r,int *g, int *b,int taille)
{
    if (!gestionnaire)
    {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans ajouter_image_au_tableau_batch()\n");
        return;
    }

    if (taille <= 0)
    {
        fprintf(stderr, "Erreur: Taille de batch invalide (%d)\n", taille);
        return;
    }

    for (int i = 0; i < taille; i++)
    {
        ajouter_forme_au_tableau(gestionnaire, x[i], y[i], w[i], h[i], sens[i], rotation[i],transparence[i],r[i],g[i],b[i]);
    }
}






 





#include <math.h>

void afficher_images(Gestionnaire* gestionnaire)
{
    if (!gestionnaire || !gestionnaire->rendu || !gestionnaire->image)
    {
        fprintf(stderr, "Erreur: Composants manquants pour afficher les images\n");
        return;
    }

    Tableau_image* jeu = gestionnaire->image;

    float coeff = (float)gestionnaire->largeur_actuel / (float)gestionnaire->largeur;

    float decalage_x_scaled = gestionnaire->decalage_x / coeff;
    float decalage_y_scaled = gestionnaire->decalage_y / coeff;

    for (int i = 0; i < jeu->nb_images; i++)
    {
        image* img = &jeu->tab[i];

        if (lroundf(img->posx - decalage_x_scaled) > gestionnaire->largeur ||
            lroundf(img->posx + img->taillex + decalage_x_scaled) < 0 || 
            lroundf(img->posy - decalage_y_scaled) > gestionnaire->hauteur ||
            lroundf(img->posy + img->tailley + decalage_y_scaled) < 0)
        {
            continue;
        }

        SDL_Rect dst = {
            (int)lroundf(img->posx * coeff + (float)gestionnaire->decalage_x),
            (int)lroundf(img->posy * coeff + (float)gestionnaire->decalage_y),
            (int)lroundf(img->taillex * coeff),
            (int)lroundf(img->tailley * coeff)
        };

        if (img->forme)
        {
            if (img->rotation == 0 && img->sens == 0)
            {
                SDL_SetRenderDrawColor(gestionnaire->rendu, img->r, img->g, img->b, img->transparence);
                SDL_SetRenderDrawBlendMode(gestionnaire->rendu, SDL_BLENDMODE_BLEND);
                
                if (SDL_RenderFillRect(gestionnaire->rendu, &dst) != 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderFillRect : %s\n", SDL_GetError());
                }
            }
            else
            {
                float rad = img->rotation * (M_PI / 180.0f);
                float cosr = cosf(rad);
                float sinr = sinf(rad);

                float cx = dst.w / 2.0f;
                float cy = dst.h / 2.0f;

                // Calcul des 4 coins
                float x0 = -cx, y0 = -cy;
                float x1 = cx,  y1 = -cy;
                float x2 = cx,  y2 = cy;
                float x3 = -cx, y3 = cy;

                SDL_Vertex vertices[4];

                // Remplissage des vertices
                float rx, ry;
                // Vertex 0
                rx = x0 * cosr - y0 * sinr + dst.x + cx;
                ry = x0 * sinr + y0 * cosr + dst.y + cy;
                if (img->sens == 1) rx = 2 * (dst.x + cx) - rx;
                vertices[0].position.x = rx;
                vertices[0].position.y = ry;
                vertices[0].color.r = img->r;
                vertices[0].color.g = img->g;
                vertices[0].color.b = img->b;
                vertices[0].color.a = img->transparence;

                // Vertex 1
                rx = x1 * cosr - y1 * sinr + dst.x + cx;
                ry = x1 * sinr + y1 * cosr + dst.y + cy;
                if (img->sens == 1) rx = 2 * (dst.x + cx) - rx;
                vertices[1].position.x = rx;
                vertices[1].position.y = ry;
                vertices[1].color.r = img->r;
                vertices[1].color.g = img->g;
                vertices[1].color.b = img->b;
                vertices[1].color.a = img->transparence;

                // Vertex 2
                rx = x2 * cosr - y2 * sinr + dst.x + cx;
                ry = x2 * sinr + y2 * cosr + dst.y + cy;
                if (img->sens == 1) rx = 2 * (dst.x + cx) - rx;
                vertices[2].position.x = rx;
                vertices[2].position.y = ry;
                vertices[2].color.r = img->r;
                vertices[2].color.g = img->g;
                vertices[2].color.b = img->b;
                vertices[2].color.a = img->transparence;

                // Vertex 3
                rx = x3 * cosr - y3 * sinr + dst.x + cx;
                ry = x3 * sinr + y3 * cosr + dst.y + cy;
                if (img->sens == 1) rx = 2 * (dst.x + cx) - rx;
                vertices[3].position.x = rx;
                vertices[3].position.y = ry;
                vertices[3].color.r = img->r;
                vertices[3].color.g = img->g;
                vertices[3].color.b = img->b;
                vertices[3].color.a = img->transparence;

                int indices[6] = {0, 1, 2, 0, 2, 3};

                if (SDL_RenderGeometry(gestionnaire->rendu, NULL, vertices, 4, indices, 6) != 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderGeometry : %s\n", SDL_GetError());
                }
            }
        }
        else
        {
            if (!img->texture)
                continue;

            SDL_SetTextureAlphaMod(img->texture, img->transparence);

            if (img->rotation == 0.0f && img->sens == 0)
            {
                if (SDL_RenderCopy(gestionnaire->rendu, img->texture, NULL, &dst) != 0)
                {
                    fprintf(stderr, "Erreur rendu texture %d: %s\n", i, SDL_GetError());
                }
            }
            else
            {
                SDL_Point centre = {dst.w / 2, dst.h / 2};
                SDL_RendererFlip flip = (img->sens == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

                if (SDL_RenderCopyEx(gestionnaire->rendu, img->texture, NULL, &dst, img->rotation, &centre, flip) != 0)
                {
                    fprintf(stderr, "Erreur rendu texture %d: %s\n", i, SDL_GetError());
                }
            }
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

void actualiser(Gestionnaire* jeu, bool bande_noir)
{
    if (!jeu || !jeu->rendu)
    {
        fprintf(stderr, "Erreur: Gestionnaire ou renderer NULL dans actualiser()\n");
        return;
    }

    SDL_Renderer* rendu = jeu->rendu;
    if(jeu->fond->colorier_frame){
    SDL_SetRenderDrawColor(rendu,jeu->fond->r,jeu->fond->g,jeu->fond->b,255);
    jeu->fond->colorier_frame = false;
    }
    else{
    SDL_SetRenderDrawColor(rendu,255, 255, 255, 255);
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
