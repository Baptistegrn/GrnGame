#include "../../../main.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * Ajuste les dimensions et la position du rectangle de destination
 * pour que la texture tournée soit centrée correctement par rapport à
 * l'originale.
 */
void ajuster_rect_texture_tournee(SDL_Texture *texture, SDL_Rect *dst, int x_ecran, int y_ecran,
                                  int w_ecran, int h_ecran, int coeff) {
    int final_w, final_h;
    SDL_QueryTexture(texture, NULL, NULL, &final_w, &final_h);

    dst->w = final_w * coeff;
    dst->h = final_h * coeff;

    /* Centrage de la texture tournée */
    dst->x = x_ecran - (dst->w - w_ecran) / 2;
    dst->y = y_ecran - (dst->h - h_ecran) / 2;
}

/*
 * Crée une nouvelle texture en appliquant une rotation pixel par pixel.
 * Utilise l'interpolation "Nearest Neighbor" pour préserver le style Pixel Art.
 * La rotation est effectuée en utilisant une transformation de coordonnées
 * inversée.
 */
static SDL_Texture *creer_texture_tournee_pixelart(SDL_Texture *src, Uint16 w, Uint16 h,
                                                   Uint16 angle, SDL_RendererFlip flip) {
    if (!gs)
        goto gsvide;
    SDL_Renderer *rendu = gs->fenetre->rendu;

    /* Calculs trigonométriques */
    double angle_rad = (double)angle * M_PI / 180.0;
    double cos_a = cos(angle_rad);
    double sin_a = sin(angle_rad);

    /* Calcul de la nouvelle taille englobante basée sur la rotation */
    int new_w = (int)ceil(fabs(w * cos_a) + fabs(h * sin_a));
    int new_h = (int)ceil(fabs(w * sin_a) + fabs(h * cos_a));

    /* Création texture temporaire pour lire les pixels source */
    SDL_Texture *temp_target =
        SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!temp_target) {
        goto gsvide;
    }

    SDL_SetRenderTarget(rendu, temp_target);
    SDL_RenderCopy(rendu, src, NULL, NULL);
    SDL_SetRenderTarget(rendu, NULL);

    Uint32 *pixels_src = (Uint32 *)xmalloc(w * h * sizeof(Uint32));

    /* Lecture des pixels depuis la texture temporaire */
    SDL_SetRenderTarget(rendu, temp_target);
    if (SDL_RenderReadPixels(rendu, NULL, SDL_PIXELFORMAT_RGBA8888, pixels_src,
                             w * sizeof(Uint32)) != 0) {
        xfree(pixels_src);
        SDL_DestroyTexture(temp_target);
        SDL_SetRenderTarget(rendu, NULL);
        goto gsvide;
    }

    SDL_SetRenderTarget(rendu, NULL);
    SDL_DestroyTexture(temp_target);

    /* Buffer pour les pixels de destination */
    Uint32 *pixels_dst = (Uint32 *)xmalloc(new_w * new_h * sizeof(Uint32));
    memset(pixels_dst, 0, new_w * new_h * sizeof(Uint32));

    double cx_src = w / 2.0;
    double cy_src = h / 2.0;
    double cx_dst = new_w / 2.0;
    double cy_dst = new_h / 2.0;

    /* Rotation manuelle: chaque pixel destination est calculé par rotation
     * inverse */
    for (int y_dst = 0; y_dst < new_h; y_dst++) {
        for (int x_dst = 0; x_dst < new_w; x_dst++) {
            /* Coordonnées relatives au centre de rotation */
            double dx = x_dst - cx_dst;
            double dy = y_dst - cy_dst;

            /* Rotation inverse pour trouver le pixel source correspondant */
            double x_src = dx * cos_a + dy * sin_a + cx_src;
            double y_src = -dx * sin_a + dy * cos_a + cy_src;

            /* Application du miroir horizontal si nécessaire */
            if (flip == SDL_FLIP_HORIZONTAL)
                x_src = w - 1 - x_src;

            int ix = (int)round(x_src);
            int iy = (int)round(y_src);

            /* Copie du pixel si les coordonnées sont valides */
            if (ix >= 0 && ix < w && iy >= 0 && iy < h)
                pixels_dst[y_dst * new_w + x_dst] = pixels_src[iy * w + ix];
        }
    }

    /* Création de la texture SDL finale avec interpolation "Nearest Neighbor" */
    SDL_Texture *texture_tournee =
        SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, new_w, new_h);
    if (texture_tournee) {
        SDL_SetTextureBlendMode(texture_tournee, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture_tournee, SDL_ScaleModeNearest);
        SDL_UpdateTexture(texture_tournee, NULL, pixels_dst, new_w * sizeof(Uint32));
    }

    xfree(pixels_src);
    xfree(pixels_dst);
    return texture_tournee;

gsvide:
    log_message(NiveauLogDebug,
                "manager is empty in creation of pixel art rotated texture function");
    return NULL;
}

/*
 * Optimise la rotation pour les angles droits (90, 180, 270 degrés).
 * Utilise une approche par permutation directe des pixels pour plus de
 * rapidité. Les angles non multiples de 90 utilisent l'algorithme complet.
 */
SDL_Texture *creer_texture_tournee_rapide(SDL_Texture *src, Uint16 w, Uint16 h, Uint16 angle,
                                          SDL_RendererFlip flip) {
    if (!gs)
        goto gsvide;

    SDL_Renderer *rendu = gs->fenetre->rendu;

    /* Normalisation de l'angle entre 0 et 360 */
    int angle_norm = (angle % 360 + 360) % 360;

    /* Si l'angle n'est pas multiple de 90, utilise l'algorithme complet */
    if (angle_norm % 90 != 0)
        return creer_texture_tournee_pixelart(src, w, h, angle, flip);

    int rotation_90 = angle_norm / 90;

    /* Création texture temporaire pour lire les pixels source */
    SDL_Texture *temp_target =
        SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!temp_target)
        goto gsvide;

    SDL_SetRenderTarget(rendu, temp_target);
    SDL_RenderCopy(rendu, src, NULL, NULL);

    Uint32 *pixels_src = (Uint32 *)xmalloc(w * h * sizeof(Uint32));
    SDL_RenderReadPixels(rendu, NULL, SDL_PIXELFORMAT_RGBA8888, pixels_src, w * sizeof(Uint32));

    SDL_SetRenderTarget(rendu, NULL);
    SDL_DestroyTexture(temp_target);

    /* Calcul des nouvelles dimensions selon la rotation */
    int new_w = (rotation_90 % 2 == 0) ? w : h;
    int new_h = (rotation_90 % 2 == 0) ? h : w;

    Uint32 *pixels_dst = (Uint32 *)xmalloc(new_w * new_h * sizeof(Uint32));

    /* Permutation rapide des pixels selon l'angle */
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int src_idx = y * w + x;
            int dst_x, dst_y;

            int fx = (flip == SDL_FLIP_HORIZONTAL) ? (w - 1 - x) : x;

            switch (rotation_90) {
            case 0: /* 0° */
                dst_x = fx;
                dst_y = y;
                break;
            case 1: /* 90° */
                dst_x = h - 1 - y;
                dst_y = fx;
                break;
            case 2: /* 180° */
                dst_x = w - 1 - fx;
                dst_y = h - 1 - y;
                break;
            case 3: /* 270° */
                dst_x = y;
                dst_y = w - 1 - fx;
                break;
            default:
                dst_x = x;
                dst_y = y;
                break;
            }

            pixels_dst[dst_y * new_w + dst_x] = pixels_src[src_idx];
        }
    }

    /* Création de la texture finale avec les pixels permutés */
    SDL_Texture *texture_tournee =
        SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, new_w, new_h);
    if (texture_tournee) {
        SDL_SetTextureBlendMode(texture_tournee, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture_tournee, SDL_ScaleModeNearest);
        SDL_UpdateTexture(texture_tournee, NULL, pixels_dst, new_w * sizeof(Uint32));
    }

    xfree(pixels_src);
    xfree(pixels_dst);
    return texture_tournee;

gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in creation of fast rotated texture function");
    return NULL;
}
