#include "all.h"
#include <SDL3/SDL_render.h>
#include <stdbool.h>

SDLContext sdl_context;

void
render_destroy(void)
{
    SDL_DestroyRenderer(sdl_context.sdl_renderer);
    SDL_DestroyWindow(sdl_context.window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

bool
render_init(size_t window_width, size_t window_height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        goto err;

    // Create Window
    bool render_and_window_init_success = SDL_CreateWindowAndRenderer(
        "Jy Jhree J Joftware Jenderer\n",
        window_width, window_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS,
        &sdl_context.window,
        &sdl_context.sdl_renderer
    );
    if (!sdl_context.window)
        goto err;

    // Initialize Timer
    sdl_context.ticks = SDL_GetTicks();
    sdl_context.window_width = window_width;
    sdl_context.window_height = window_height;
    return true;

err:
    render_destroy();
    return false;
}

bool
render_resize(size_t new_width, size_t new_height)
{
    sdl_context.window_width = new_width;
    sdl_context.window_height = new_height;

    return true;
}

void
render_set_draw_colour(Colour c)
{
    u8 a, r, g, b;
    b = c & 255;
    g = (c >>= 8) & 255;
    r = (c >>= 8) & 255;
    a = (c >>= 8) & 255;

    SDL_SetRenderDrawColor(
        sdl_context.sdl_renderer,
        r, g, b, a
    );
}

void
render_begin(void)
{
    int pitch = sdl_context.window_width * 4;

    render_set_draw_colour(0);
    SDL_RenderClear(sdl_context.sdl_renderer);

    sdl_context.ticks = SDL_GetTicks();
}

void
render_draw_grid()
{
}

void
render_end(void)
{
    SDL_RenderPresent(sdl_context.sdl_renderer);

    float sleep_time =
        ((int) (1.0f / 144.0f * 1000.0f)) - (SDL_GetTicks() - sdl_context.ticks);

    if (sleep_time > 0)
        SDL_Delay(sleep_time);
}
