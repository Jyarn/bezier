#include "all.h"

void
glyph_draw(i64 grid_size)
{
    assert(grid_size > 0);

    render_begin();
    for (int x = 0; x < sdl_context.window_width; x++)
    {
        for (int y = 0; y < sdl_context.window_height; y++)
        {
            if (x % grid_size == 0 || y % grid_size == 0)
            {
                render_draw_pixel_at(0x220000, x, y);
            }
            else
            {
                render_draw_pixel_at(0, x, y);
            }

            for (i64 i = 0; i < num_control_points; i++)
            {
                i64 dist_x = x - control_points[i].x;
                i64 dist_y = y - control_points[i].y;
                i64 dist = dist_x * dist_x + dist_y * dist_y;

                if (dist < 16)
                {
                    render_draw_pixel_at(
                        i == focused_index ?
                            0x00FF00 : 0xFF0000,
                        x,
                        y
                    );
                    break;
                }
            }
        }
    }
    render_end();
}
