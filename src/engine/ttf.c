#include "ttf.h"
#include "../main.h"
#include "../moteur/fenetre/fenetre.h"
#include "../moteur/logging/logging.h"
#include "SDL_pixels.h"
#include "SDL_platform.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ensure_ttf_init() {
    static bool ttf_init = false;
    if (!ttf_init) {
        TTF_Init();
        ttf_init = true;
    }
}

typedef struct {
    char path[256];
    int size;
    TTF_Font *font;
} FontEntry;

static FontEntry fonts[256];
static int fonts_n = 0;

TTF_Font *ensure_loaded(const char *font, int size) {
    for (int i = 0; i < fonts_n; i++) {
        if (strcmp(fonts[i].path, font) == 0 && fonts[i].size == size) {
            return fonts[i].font;
        }
    }

    TTF_Font *ttffont = TTF_OpenFont(font, size);
    strncpy(fonts[fonts_n].path, font, 255);
    fonts[fonts_n].size = size;
    fonts[fonts_n].font = ttffont;
    fonts_n++;
    return ttffont;
}

typedef struct {
    char font[256];
    char text[256];
    int size, x, y, r, g, b, a;
} TTFDrawCall;

static TTFDrawCall
    draw_queue[1024]; // we cannot draw direct because sir baptistegrn make actualiser boucle
static int draw_queue_n = 0;

void draw_ttf(const char *font, const char *text, int size, int x, int y, int r, int g, int b,
              int a) {
    ensure_ttf_init();
    ensure_loaded(font, size);

    TTFDrawCall *call = &draw_queue[draw_queue_n++];
    strncpy(call->font, font, 255);
    strncpy(call->text, text, 255);
    call->size = size;
    call->x = x;
    call->y = y;
    call->r = r;
    call->g = g;
    call->b = b;
    call->a = a;
}

void update_ttf() {
    for (int i = 0; i < draw_queue_n; i++) {
        TTFDrawCall *ttfdraw = &draw_queue[i];
        TTF_Font *ttfont = ensure_loaded(ttfdraw->font, ttfdraw->size);
        SDL_Color color = {ttfdraw->r, ttfdraw->g, ttfdraw->b, ttfdraw->a};
        SDL_Surface *surface =
            TTF_RenderUTF8_Blended(ttfont, ttfdraw->text, color); // utf8 so hindi work
        SDL_Texture *texture = SDL_CreateTextureFromSurface(gs->fenetre->rendu, surface);
        SDL_Rect dst = {ttfdraw->x, ttfdraw->y, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
        SDL_RenderCopy(gs->fenetre->rendu, texture, NULL, &dst);
    }
    draw_queue_n = 0;
}