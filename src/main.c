#include "all.h"

int
main(void)
{
    render_init(1920, 1080);

    u32 mouse_pressed_mask;
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    bool is_dragging_mouse = false;

    control_points = malloc(3 * sizeof(ControlPoint));
    control_points[0] = (ControlPoint) { 100, 100 };
    control_points[1] = (ControlPoint) { 500, 500 };
    control_points[2] = (ControlPoint) { 900, 900 };

    num_control_points = 3;

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    goto exit;
                case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        render_resize(event.window.data1, event.window.data2);
                    }
                    break;
                }
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            goto exit;
                        case SDL_SCANCODE_RETURN:
                        {
                            control_points = realloc(control_points, (num_control_points + 1) * sizeof(ControlPoint));
                            control_points[num_control_points] = (ControlPoint)
                            {
                                .x = mouse_x,
                                .y = mouse_y
                            };
                            num_control_points += 1;
                            break;
                        }
                        case SDL_SCANCODE_DELETE:
                        {
                            if (focused_index == -1) break;
                            for (int i = focused_index; i < num_control_points - 1; i++)
                            {
                                control_points[i] = control_points[i + 1];
                            }
                            control_points = realloc(control_points, (num_control_points - 1) * sizeof(ControlPoint));
                            num_control_points -= 1;
                            focused_index = -1;
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
            }
        }


        mouse_pressed_mask = SDL_GetMouseState(&mouse_x, &mouse_y);

        if (mouse_pressed_mask & SDL_BUTTON(1))
        {
            if (!is_dragging_mouse)
            {
                int i = 0;
                while (true)
                {
                    if (i == num_control_points)
                    {
                        focused_index = -1;
                        is_dragging_mouse = false;
                        break;
                    }

                    i64 dist_x = control_points[i].x - mouse_x;
                    i64 dist_y = control_points[i].y - mouse_y;

                    i64 dist = dist_x * dist_x + dist_y * dist_y;

                    if (dist < 25)
                    {
                        focused_index = i;
                        is_dragging_mouse = true;
                        break;
                    }

                    i += 1;
                }
            }
            else
            {
                control_points[focused_index].x = mouse_x;
                control_points[focused_index].y = mouse_y;
            }
        }
        else
        {
            is_dragging_mouse = false;
        }

        u64 last_tick = sdl_context.ticks;
        render_begin();
        glyph_draw(20);
        render_end();
        printf("\x1b[2K\r%ldms", sdl_context.ticks - last_tick);
        fflush(stdout);
    }
exit:
    printf("\n");
    render_destroy();
    return 0;
}
