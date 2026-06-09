#include "sprite.h"
#include "SDL3/SDL_surface.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/math/math.h"
#include "grngame/utils/attributes.h"
#include "kvec.h"
#include "renderer.h"
#include "texture.h"
#include <math.h>

HOT bool SpriteDraw(Sprite s, uint16 frame, float x, float y, uint8 c, int16 r, uint8 a)
{
    if (s.tex == NULL)
        s.tex = TextureGet(s.name);

    if (s.tex == NULL)
    {
        LOG_WARNING("Impossible to get texture : %s", s.name);
        return false;
    }

    Texture *tex = s.tex;

    int16 tex_w = tex->w;

    if (OffScreen(x, y, (float32)(tex_w * c), (float32)(tex->h * c)))
    {
        // todo log
        return false;
    }

    int16 cols = tex_w / s.w;
    if (cols <= 0)
    {
        LOG_WARNING("Impossible to get case : %d ", frame);
        return false;
    }

    SDL_FRect src = {(float32)((frame % cols) * s.w), (float32)((frame / cols) * s.h), (float32)s.w, (float32)s.h};
    SDL_FRect dst = {PIXEL_ALIGN(x + g_app.info.offset_x), PIXEL_ALIGN(y + g_app.info.offset_y), (float32)(s.w * c),
                     (float32)(s.h * c)};
    SDL_FPoint center = {dst.w / 2.0f, dst.h / 2.0f};

    RendererSetTextureAlpha(tex->texture, a);
    if (r == 0)
    {

        SDL_RenderTexture(g_app.renderer.renderer, tex->texture, &src, &dst);
    }
    else
    {
        RendererTextureRotated(tex->texture, &src, &dst, (float64)r, &center, SDL_FLIP_NONE);
    }

    return true;
}