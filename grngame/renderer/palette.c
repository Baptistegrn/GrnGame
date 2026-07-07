#include "palette.h"
#include "SDL3/SDL_pixels.h"
#include "cielab.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/wren_get.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/cielab.h"
#include "kvec.h"
#include "wren.h"
#include <math.h>

void PaletteInit()
{
    kv_init(g_app.info.palette_elements);
    kv_init(g_app.info.palette_elements_lab);
}

void PaletteDestroy()
{
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
    int32 size = WrenGetListCount("config", "Config", "colorPalette");
    for (int32 i = 0; i < size; i++)
    {
        const char *color = WrenGetListString("config", "Config", "colorPalette", i);
        if (!LIKELY(color == NULL))
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