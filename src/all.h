#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
#include <SDL3/SDL.h>

typedef uint32_t Colour;

typedef struct
{
    int window_width, window_height;
    uint64_t ticks;
    SDL_Window *window;
    SDL_Renderer *sdl_renderer;
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


/* *
 * src/global.c
 */
extern i64 num_control_points;
extern ControlPoint *control_points;
extern i64 focused_index;


/* *
 * src/arena.c
 */
void *arena_alloc(i64 size, i64 n_members);
void arena_free(u8 *ptr);
void arena_reset(void);


/* *
 * renderers/
 */
typedef enum
{
    None,
    Scanline_1,
} Renderer;

typedef void (*RenderFunc) (ControlPoint* points, i64 n_points, i64 grid_size, float *grid_out);

extern RenderFunc renderer_none;
extern RenderFunc renderer_scanline_1;

void render_set_draw_colour(Colour c);


/* *
 * Macros
 */
#define MB * 1048576
#define KB * 1024
#define PAGES * 4096
