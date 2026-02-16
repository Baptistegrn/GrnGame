/*
 * Rotation de textures avec cache et optimisation.
 */

#include "rotation.h"
#include "../../../main.h"
#include "../../fenetre/fenetre.h"
#include "../../logging/logging.h"

/* Ajuste les dimensions et la position du rectangle de destination pour centrer la texture tournee
 */
void ajuster_rect_texture_tournee(SDL_Texture *texture, SDL_Rect *dst, int x_ecran, int y_ecran,
                                  int w_ecran, int h_ecran, int coeff) {
    int final_w, final_h;
    SDL_QueryTexture(texture, NULL, NULL, &final_w, &final_h);

    dst->w = final_w * coeff;
    dst->h = final_h * coeff;

    /* Centrage de la texture tournee */
    dst->x = x_ecran - (dst->w - w_ecran) / 2;
    dst->y = y_ecran - (dst->h - h_ecran) / 2;
}

/* creer une texture selon langle donnee en pixel perfect */
SDL_Texture *creer_texture_angle(SDL_Texture *src, Uint16 w, Uint16 h, Uint16 angle,
                                 SDL_RendererFlip flip) {
    if (!gs)
        goto gsvide;

    SDL_Renderer *rendu = gs->fenetre->rendu;

    /* Taille englobante */
    int size = (int)ceil(sqrt(w * w + h * h));

    SDL_Texture *dst =
        SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size, size);
    if (!dst)
        return NULL;

    SDL_SetTextureBlendMode(dst, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(dst, SDL_ScaleModeNearest);

    SDL_SetRenderTarget(rendu, dst);

    /* Clear avec couleur transparente */
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 0);
    SDL_RenderClear(rendu);

    /* Centrage de la source */
    SDL_Rect rect = {(size - w) / 2, (size - h) / 2, w, h};

    /* Sauvegarder et reset le blend mode et alpha pour copie directe */
    SDL_BlendMode ancien_blend;
    Uint8 ancien_alpha;
    SDL_GetTextureBlendMode(src, &ancien_blend);
    SDL_GetTextureAlphaMod(src, &ancien_alpha);

    /* valeurs par defaut */
    SDL_SetTextureBlendMode(src, SDL_BLENDMODE_NONE);
    SDL_SetTextureAlphaMod(src, 255);

    SDL_RenderCopyEx(rendu, src, NULL, &rect, angle, NULL, flip);

    /* Restaurer les valeurs originales */
    SDL_SetTextureBlendMode(src, ancien_blend);
    SDL_SetTextureAlphaMod(src, ancien_alpha);
    SDL_SetRenderTarget(rendu, NULL);
    return dst;
gsvide:
    log_message(NiveauLogDebug, "manager is empty in create rotated texture");
    return NULL;
}
