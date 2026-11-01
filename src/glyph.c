#include "all.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>

float *intensities = NULL;
i64 n_intensities = 0;
i64 grid_size = 100;
i64 control_point_radius = 10;

void
glyph_draw(void)
{
    render_begin();

    int ww = sdl_context.window_width;
    int wh = sdl_context.window_height;

    if (grid_size != 0)
    {
        intensities = arena_alloc(sizeof(float), ww / grid_size * wh / grid_size);
        int i = 0;
        for (int x = 0; x < ww; x++)
        {
            for (int y = 0; y < wh; y++)
            {
                SDL_FRect rect =
                {
                    .x = x * grid_size + 1,
                    .y = y * grid_size + 1,
                    .w = grid_size - 1,
                    .h = grid_size - 1,
                };
                SDL_SetRenderDrawColor(
                    sdl_context.sdl_renderer,
                    rand() % 255, 0, 0, SDL_ALPHA_OPAQUE
                );
                SDL_RenderFillRect(
                    sdl_context.sdl_renderer,
                    &rect
                );

                i += 1;
            }
        }
    }

    if (num_control_points != 0)
    {

        render_set_draw_colour(-1);
        for (int i = 0; i < num_control_points-1; i++)
        {
            SDL_RenderLine(
                sdl_context.sdl_renderer,
                control_points[i].x, control_points[i].y,
                control_points[i+1].x, control_points[i+1].y
            );
        }
        SDL_RenderLine(
            sdl_context.sdl_renderer,
            control_points[num_control_points-1].x, control_points[num_control_points-1].y,
            control_points[0].x, control_points[0].y
        );

        for (int i = 0; i < num_control_points; i++)
        {
            ControlPoint point = control_points[i];
            SDL_FRect rect =
            {
                .x = point.x - control_point_radius,
                .y = point.y - control_point_radius,
                .w = control_point_radius * 2,
                .h = control_point_radius * 2,
            };

            if (i == focused_index)
                render_set_draw_colour(0x0000ff);
            else
                render_set_draw_colour(0x00ff00);

            SDL_RenderFillRect(
                sdl_context.sdl_renderer,
                &rect
            );
        }
    }
    arena_reset();
    render_end();
}
