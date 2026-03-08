#include "sprite.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "renderer.h"
#include "texture.h"

bool SpriteDraw(Sprite s, uint16 frame, float x, float y, uint8 c, bool f, int16 r, uint8 a)
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

    int16 cols = tex_w / s.w;
    if (cols <= 0)
    {
        LOG_WARNING("Impossible to get case : %d ", frame);
        return false;
    }

    SDL_FRect src = {(float32)((frame % cols) * s.w), (float32)((frame / cols) * s.h), (float32)s.w, (float32)s.h};
    SDL_FRect dst = {x + g_app.info.offset_x, y + g_app.info.offset_y, (float32)(s.w * c), (float32)(s.h * c)};
    SDL_FPoint center = {dst.w / 2.0f, dst.h / 2.0f};

    RendererSetTextureAlpha(tex->texture, a);

    SDL_FlipMode flip_mode = f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    RendererTextureRotated(tex->texture, &src, &dst, (float64)r, &center, flip_mode);

    return true;
}
