#include "all.h"
#include <SDL2/SDL_render.h>
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
    if (SDL_Init(SDL_INIT_VIDEO))
        goto err;

    // Create Window
    sdl_context.window = SDL_CreateWindow(
        "Jy Jhree J Joftware Jenderer\n",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_RESIZABLE
    );
    if (!sdl_context.window)
        goto err;

    // Initialize Renderer
    sdl_context.sdl_renderer = SDL_CreateRenderer(
        sdl_context.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!sdl_context.sdl_renderer)
        goto err;

    // Initialize Texture
    sdl_context.texture = SDL_CreateTexture(
        sdl_context.sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width, window_height
    );
    if (!sdl_context.texture)
        goto err;

    // Initialize Timer
    sdl_context.ticks = SDL_GetTicks64();
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

    sdl_context.texture = SDL_CreateTexture(
        sdl_context.sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        new_width, new_height
    );
    if (sdl_context.texture == NULL)
        return false;

    return true;
}

void
render_begin(void)
{
    int pitch = sdl_context.window_width * 4;

    int succ = SDL_LockTexture(
        sdl_context.texture,
        NULL,
        (void**) &sdl_context.colour_buffer,
        &pitch
    );
    memset(
        sdl_context.colour_buffer,
        0,
        sdl_context.window_width * sdl_context.window_height * 4
    );

    static_assert(sizeof(int) == sizeof(float), "");

    sdl_context.ticks = SDL_GetTicks64();
}

void
render_end(void)
{
    // Unlock Texture
    SDL_UnlockTexture(sdl_context.texture);

    // Copy Texture And Render To Screen
    SDL_RenderCopy(
        sdl_context.sdl_renderer,
        sdl_context.texture,
        NULL,
        NULL
    );
    SDL_RenderPresent(sdl_context.sdl_renderer);

    // Reset
    sdl_context.colour_buffer = NULL;
    float sleep_time =
        ((int) (1.0f / 144.0f * 1000.0f)) - (SDL_GetTicks64() - sdl_context.ticks);

    if (sleep_time > 0)
        SDL_Delay(sleep_time);
}

void
render_draw_pixel_at(Colour colour, int x, int y)
{
    if (!sdl_context.colour_buffer)
        return;
    else if (x < 0 || x >= sdl_context.window_width)
        return;
    else if (y < 0 || y >= sdl_context.window_height)
        return;

    size_t idx = y * sdl_context.window_width + x;
    assert(idx < sdl_context.window_width * sdl_context.window_height);
    sdl_context.colour_buffer[idx] = colour;
}
