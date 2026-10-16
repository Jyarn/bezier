#include "all.h"

Colour *base_grid_image = NULL;
float *intensities = NULL;
i64 n_intensities = 0;

static void
draw_line(ControlPoint src, ControlPoint dest, Colour line_colour)
{
    int dx = dest.x - src.x;
    int dy = dest.y - src.y;

    if (dx == 0)
    {
        int sy = dy > 0 ? 1 : -1;
        for (int i = 0; i < abs(dy); i++)
        {
            render_draw_pixel_at(line_colour, src.x, src.y);
            src.y += sy;
        }
    }
    else if (dy == 0)
    {
        int sx = dx > 0 ? 1 : -1;
        for (int i = 0; i < abs(dx); i++)
        {
            render_draw_pixel_at(line_colour, src.x, src.y);
            src.x += sx;
        }
    }
    else if (abs(dx) > abs(dy))
    {
        int sx = dx > 0 ? 1 : -1;               // shift x
        float sy = (float) dy / (float) abs(dx);// shift y

        int x = src.x;                          // current x position
        float y = (float) src.y;                // current y position

        for (int i = 0; i < abs(dx); i++)
        {
            render_draw_pixel_at(
                line_colour,
                x, roundf(y)
            );
            x += sx;
            y += sy;
        }

        assert(x == dest.x);
    }
    else
    {
        float sx = (float) dx / (float) abs(dy);// shift x
        int sy = dy > 0 ? 1 : -1;               // shift y

        float x = (float) src.x;                // current x position
        int y = src.y;                          // currernt y position

        for (int i = 0; i < abs(dy); i++)
        {
            render_draw_pixel_at(
                line_colour,
                roundf(x), y
            );
            x += sx;
            y += sy;
        }

        assert(y == dest.y);
    }
}

void
glyph_recreate_grid(i64 new_size, Colour grid_colour)
{
    base_grid_image = realloc(
        base_grid_image,
        sdl_context.window_width * sdl_context.window_height * sizeof(Colour)
    );
    memset(base_grid_image, 0, sdl_context.window_width * sdl_context.window_height * sizeof(Colour));

    for (int i = 0; i < sdl_context.window_width; i += new_size)
    {
        base_grid_image[i] = grid_colour;
    }

    for (int i = 0; i < sdl_context.window_height; i++)
    {
        memcpy(
            &base_grid_image[i * sdl_context.window_width],
            base_grid_image,
            sdl_context.window_width * sizeof(Colour)
        );
    }

    for (int i = 0; i < sdl_context.window_height; i += new_size)
    {
        for (int j = 0; j < sdl_context.window_width; j++)
        {
            base_grid_image[i * sdl_context.window_width + j] = grid_colour;
        }
    }
}

void
glyph_draw(void)
{
    render_begin();

    memcpy(
        sdl_context.colour_buffer,
        base_grid_image,
        sdl_context.window_width * sdl_context.window_height * sizeof(Colour)
    );
    if (num_control_points != 0)
    {
        int ww = sdl_context.window_width;
        int wh = sdl_context.window_height;
        for (int i = 0; i < num_control_points; i++)
        {
            ControlPoint current_point = control_points[i];
            for (int x = current_point.x - 5; x < ww && x < current_point.x + 5; x++)
            {
                for (int y = current_point.y - 5; y < wh && y < current_point.y + 5; y++)
                {
                    int dist_x = x - current_point.x;
                    int dist_y = y - current_point.y;
                    int dist = dist_x * dist_x + dist_y * dist_y;

                    if (dist < 25)
                        sdl_context.colour_buffer[y * ww + x] =
                            i == focused_index ? 0x00FF00 : 0xFF0000;
                }
            }
        }
        for (int i = 0; i < num_control_points-1; i++)
        {
            draw_line(control_points[i], control_points[i+1], -1);
        }
        draw_line(control_points[num_control_points - 1], control_points[0], -1);
    }
    render_end();
}
