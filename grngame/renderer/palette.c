#include "palette.h"
#include "SDL3/SDL_pixels.h"
#include "cielab.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/wren_get.h"
#include "grngame/core/app.h"
#include "grngame/data/json.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/cielab.h"
#include "kvec.h"
#include "wren.h"
#include <math.h>

// todo move

static void StringVecDestroy(string_vec_t *vec)
{
    if (UNLIKELY(vec == NULL))
        return;

    for (size_t i = 0; i < kv_size(*vec); i++)
        free(kv_A(*vec, i));

    kv_destroy(*vec);
}

void PaletteInit()
{
    string_vec_t palette;
    if (!JsonGetStringArray("config.json", "Config.palette", &palette))
    {
        LOG_ERROR("Using default value for : Config.palette");
    }
    g_app.info.palette = palette;
    kv_init(g_app.info.palette_elements);
    kv_init(g_app.info.palette_elements_lab);
}

void PaletteDestroy()
{
    StringVecDestroy(&g_app.info.palette);
    kv_destroy(g_app.info.palette_elements);
    kv_destroy(g_app.info.palette_elements_lab);
}

void PaletteAddColor(SDL_Color color)
{
    kv_push(SDL_Color, g_app.info.palette_elements, color);
    kv_push(ColorLAB, g_app.info.palette_elements_lab, RgbToLab(&color));
}

void HexPaletteHashInit()
{
    kh_init(ColorHex);
}

static Uint8 HexToU8(const char *hex)
{
    char buf[3] = {hex[0], hex[1], 0};
    return (Uint8)strtol(buf, NULL, 16);
}

SDL_Color ColorFromHex(const char *hex)
{
    SDL_Color color = {255, 255, 255, 255};

    if (hex[0] == '#')
        hex++;

    khash_t(ColorHex) *h = &g_app.info.palette_hex_hash;

    khiter_t k = kh_get(ColorHex, h, hex);

    if (k != kh_end(h))
    {
        return kh_value(h, k);
    }

    color.r = HexToU8(hex);
    color.g = HexToU8(hex + 2);
    color.b = HexToU8(hex + 4);
    color.a = 255;

    int32 ret;
    k = kh_put(ColorHex, h, hex, &ret);
    kh_value(h, k) = color;

    return color;
}

void PaletteParse()
{
    string_vec_t *text = &g_app.info.palette;
    if (text == NULL || kv_size(*text) == 0)
    {
        LOG_INFO("No color palette detected, set to black and white");
        PaletteAddColor((SDL_Color){0, 0, 0, 255});
        PaletteAddColor((SDL_Color){255, 255, 255, 255});
        return;
    }

    for (size_t i = 0; i < kv_size(*text); i++)
    {
        const char *color = kv_A(*text, i);
        if (LIKELY(color != NULL))
        {
            PaletteAddColor(ColorFromHex(color));
        }
    }
}

void PaletteReload()
{
    PaletteDestroy();
    PaletteInit();
    PaletteParse();
    ReloadAllTexturesWithPalette();
}
