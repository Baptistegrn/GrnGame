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

int CompareInt(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void SortPaletteAlphas()
{
    if (g_app.info.palette_alpha.n > 1)
    {
        qsort(g_app.info.palette_alpha.a, g_app.info.palette_alpha.n, sizeof(int), CompareInt);
    }
}

void LoadDefaultAlphas()
{
    size_t count = sizeof(default_alphas) / sizeof(default_alphas[0]);

    for (size_t i = 0; i < count; i++)
    {
        kv_push(int, g_app.info.palette_alpha, default_alphas[i]);
    }
}

void ParseAlphaFileContent(char *content)
{
    char *line = strtok(content, "\r\n");
    while (line != NULL)
    {
        int alpha_val;

        if (sscanf(line, "%d", &alpha_val) == 1)
        {
            if (alpha_val >= 0 && alpha_val <= 255)
            {
                kv_push(int, g_app.info.palette_alpha, alpha_val);
            }
            else
            {
                LOG_WARNING("Alpha value out of bounds (0-255) ignored: %d", alpha_val);
            }
        }
        else
        {
            LOG_WARNING("Invalid alpha line ignored: %s", line);
        }

        line = strtok(NULL, "\r\n");
    }
}

void LoadPaletteAlpha()
{
    char *content = FetchPaletteContent(PALETTE_ALPHA_FILE);

    if (content != NULL)
    {
        ParseAlphaFileContent(content);
        free(content);
    }
    else
    {
        LOG_WARNING("Palette alpha file doesn't exist, default alpha is used");
        LoadDefaultAlphas();
    }

    SortPaletteAlphas();
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
    kv_init(g_app.info.palette_alpha);
    kv_init(g_app.info.palette_elements_lab);
}

void LoadAllPalettes()
{
    LoadPaletteColor();
    LoadPaletteColorLab();
    LoadPaletteAlpha();
}

void RemoveAllPalettes()
{
    kv_destroy(g_app.info.palette_elements);
    kv_destroy(g_app.info.palette_alpha);
    kv_destroy(g_app.info.palette_elements_lab);
}

SDL_Color FindClosestPaletteColor(uint8 r, uint8 g, uint8 b, uint8 rb, uint8 gb, uint8 bb)
{
    uint64 size = kv_size(g_app.info.palette_elements);

    if (size == 0)
    {
        return (SDL_Color){rb, gb, bb, 255};
    }

    SDL_Color closest = kv_A(g_app.info.palette_elements, 0);

    int best_diff =
        (r - closest.r) * (r - closest.r) + (g - closest.g) * (g - closest.g) + (b - closest.b) * (b - closest.b);

    for (uint64 i = 1; i < size; i++)
    {
        SDL_Color current = kv_A(g_app.info.palette_elements, i);

        int diff =
            (r - current.r) * (r - current.r) + (g - current.g) * (g - current.g) + (b - current.b) * (b - current.b);

        if (diff < best_diff)
        {
            best_diff = diff;
            closest = current;
        }
    }

    closest.a = 255;
    return closest;
}

int FindClosestPaletteColorIndex(uint8 r, uint8 g, uint8 b)
{
    uint64 count = kv_size(g_app.info.palette_elements);

    if (count == 0)
    {
        return -1;
    }

    SDL_Color closest = kv_A(g_app.info.palette_elements, 0);
    int closest_index = 0;

    int best_diff =
        (r - closest.r) * (r - closest.r) + (g - closest.g) * (g - closest.g) + (b - closest.b) * (b - closest.b);

    for (uint64 i = 1; i < count; i++)
    {
        SDL_Color current = kv_A(g_app.info.palette_elements, i);

        int diff =
            (r - current.r) * (r - current.r) + (g - current.g) * (g - current.g) + (b - current.b) * (b - current.b);

        if (diff < best_diff)
        {
            best_diff = diff;
            closest_index = (int)i;
        }
    }

    return closest_index;
}