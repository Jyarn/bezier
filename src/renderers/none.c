#include "../all.h"

void
none(ControlPoint *points, i64 n_points, i64 grid_size, float *grid_out)
{
    memset(
        grid_out,
        0,
        sdl_context.window_width * sdl_context.window_height * sizeof(float)
    );
}

RenderFunc renderer_none = none;
