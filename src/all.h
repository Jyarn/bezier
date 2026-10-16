#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef int64_t i64;
typedef uint64_t u64;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef int16_t i16;
typedef float f32;
typedef double f64;
typedef uint8_t u8;

typedef const char *CString;


/* *
 * src/render.c
 */
#include <SDL2/SDL.h>

typedef uint32_t Colour;

typedef struct
{
    int window_width, window_height;
    uint64_t ticks;
    SDL_Window *window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture *texture;
    Colour *colour_buffer;
} SDLContext;

extern SDLContext sdl_context;

void render_destroy(void);
bool render_init(size_t window_width, size_t window_height);
void render_begin(void);
void render_end(void);
void render_draw_pixel_at(Colour colour, int x, int y);
bool render_resize(size_t new_width, size_t new_height);


/* *
 * src/glyph.c
 */
typedef struct
{
    i64 x, y;
} ControlPoint;

void glyph_draw(void);
void glyph_recreate_grid(i64 new_size, Colour grid_colour);


/* *
 * src/global.c
 */
extern i64 num_control_points;
extern ControlPoint *control_points;
extern i64 focused_index;
