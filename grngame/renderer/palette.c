#include "palette.h"
#include "SDL3/SDL_pixels.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/file.h"
#include "grngame/utils/string_compat.h"
#include "kvec.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static char *FetchPaletteContent(const char *filename)
{
    if (g_app.info.embedded_assets)
    {
        for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
        {
            if (strcmp(filename, g_app.info.embedded_assets[i].name) == 0)
            {
                const char *data = (const char *)g_app.info.embedded_assets[i].data;
                size_t len = strlen(data);

                char *copy = (char *)malloc(len + 1);
                if (copy)
                {
                    strcpy(copy, data);
                    return copy;
                }
            }
        }
    }

    char *path = FindFilePathFromName(filename);
    if (path)
    {
        char *content = ReturnFileString(path);
        free(path);
        return content;
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

void LoadAllPalettes()
{
    kv_init(g_app.info.palette_elements);
    kv_init(g_app.info.palette_alpha);

    LoadPaletteColor();
    LoadPaletteAlpha();
}