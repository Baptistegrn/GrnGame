#include "primitives.h"
#include "SDL_rect.h"
#include "SDL_render.h"

void dessiner_rectangle(SDL_Renderer* renderer, int x, int y, int largeur, int hauteur, uint8_t r, uint8_t g, uint8_t b,
                        uint8_t a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect rect = {x, y, largeur, hauteur};
    SDL_RenderFillRect(renderer, &rect);
}

void dessiner_cercle(SDL_Renderer* renderer, int x, int y, int rayon, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int offset_x = 0;
    int offset_y = rayon;
    int d = rayon - 1;

    while (offset_y >= offset_x)
    {
        SDL_RenderDrawLine(renderer, x - offset_y, y + offset_x, x + offset_y, y + offset_x);
        SDL_RenderDrawLine(renderer, x - offset_x, y + offset_y, x + offset_x, y + offset_y);
        SDL_RenderDrawLine(renderer, x - offset_x, y - offset_y, x + offset_x, y - offset_y);
        SDL_RenderDrawLine(renderer, x - offset_y, y - offset_x, x + offset_y, y - offset_x);

        if (d >= 2 * offset_x)
        {
            d -= 2 * offset_x + 1;
            offset_x += 1;
        }
        else if (d < 2 * (rayon - offset_y))
        {
            d += 2 * offset_y - 1;
            offset_y -= 1;
        }
        else
        {
            d += 2 * (offset_y - offset_x - 1);
            offset_y -= 1;
            offset_x += 1;
        }
    }
}
