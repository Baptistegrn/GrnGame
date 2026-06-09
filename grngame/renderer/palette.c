#include "palette.h"
#include "SDL3/SDL_pixels.h"
#include "cielab.h"
#include "grngame/assets/load.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/cielab.h"
#include "grngame/utils/file.h"
#include "grngame/utils/string_compat.h"
#include "kvec.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static char *FetchPaletteContent(const char *filename)
{
    if (g_app.info.embedded_assets_data)
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset(filename);

        if (asset)
        {
            const char *data = (const char *)asset->data;
            size_t len = strlen(data);

            char *copy = (char *)malloc(len + 1);
            if (copy)
            {
                strcpy(copy, data);
                return copy;
            }
        }
    }
    else
    {
        char *path = PathFromExecutableDirectory(filename);
        if (path)
        {
            char *content = ReturnFileString(path);
            free(path);
            return content;
        }
    }

    return NULL;
}

void LoadDefaultColors()
{
    size_t color_count = sizeof(default_colors) / sizeof(default_colors[0]);
    for (size_t i = 0; i < color_count; i++)
    {
        kv_push(SDL_Color, g_app.info.palette_elements, default_colors[i]);
    }
}

void ParseColorFileContent(char *content)
{
    //\x1A is a old windows delimiter
    char *line = strtok(content, "\r\n\x1A");
    while (line != NULL)
    {
        int r, g, b;

        if (sscanf(line, "%d,%d,%d", &r, &g, &b) == 3)
        {
            SDL_Color color = {(Uint8)r, (Uint8)g, (Uint8)b, 255};
            kv_push(SDL_Color, g_app.info.palette_elements, color);
        }
        else
        {
            LOG_WARNING("Invalid color format ignored: %s", line);
        }

        line = strtok(NULL, "\r\n\x1A");
    }
}

void LoadPaletteColor()
{
    char *content = FetchPaletteContent(PALETTE_COLOR_FILE);

    if (content != NULL)
    {
        ParseColorFileContent(content);
        free(content);
    }
    else
    {
        LOG_WARNING("Palette color file doesn't exist, default palette is used");
        LoadDefaultColors();
    }
}

static void LoadPaletteColorLab()
{
    for (uint64 i = 0; i < kv_size(g_app.info.palette_elements); i++)
    {
        SDL_Color color = kv_A(g_app.info.palette_elements, i);
        kv_push(ColorLAB, g_app.info.palette_elements_lab, RgbToLab(&color));
    }
}

void InitPalettesArrays()
{
    kv_init(g_app.info.palette_elements);
    kv_init(g_app.info.palette_elements_lab);
}

void LoadAllPalettes()
{
    LoadPaletteColor();
    LoadPaletteColorLab();
}

void RemoveAllPalettes()
{
    kv_destroy(g_app.info.palette_elements);
    kv_destroy(g_app.info.palette_elements_lab);
}
