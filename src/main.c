#include "all.h"

int
main(void)
{
    render_init(1920, 1080);

    u32 mouse_pressed_mask;
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    bool is_dragging_mouse = false;
    glyph_recreate_grid(20, 0x220000);

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
                        glyph_recreate_grid(20, 0x220000);
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
                            if (focused_index == -1)
                            {
                                if (num_control_points == 0)
                                {
                                    control_points = realloc(control_points, sizeof(ControlPoint));
                                    control_points[0] = (ControlPoint)
                                    {
                                        .x = mouse_x,
                                        .y = mouse_y
                                    };
                                    num_control_points += 1;
                                    focused_index = 0;
                                }
                                break;
                            }

                            i64 next_index = focused_index + 1;
                            i64 prev_index = focused_index - 1;

                            if (next_index >= num_control_points)
                                next_index = 0;

                            if (prev_index < 0)
                                prev_index = num_control_points - 1;

                            ControlPoint current_point = control_points[focused_index];
                            ControlPoint prev_point = control_points[prev_index];
                            ControlPoint next_point = control_points[next_index];

                            float focused_to_mouse_x = mouse_x - current_point.x;
                            float focused_to_mouse_y = mouse_y - current_point.y;

                            float focused_to_prev_x = prev_point.x - current_point.x;
                            float focused_to_prev_y = prev_point.y - current_point.y;
                            float focused_to_prev_norm = sqrtf(
                                focused_to_prev_x * focused_to_prev_x +
                                focused_to_prev_y * focused_to_prev_y
                            );

                            float focused_to_next_x = next_point.x - current_point.x;
                            float focused_to_next_y = next_point.y - current_point.y;
                            float focused_to_next_norm = sqrtf(
                                focused_to_next_x * focused_to_next_x +
                                focused_to_next_y * focused_to_next_y
                            );

                            float cos_mouse_to_prev =
                                focused_to_mouse_x * focused_to_prev_x +
                                focused_to_mouse_y * focused_to_prev_y;
                            cos_mouse_to_prev /= focused_to_next_norm;

                            float cos_mouse_to_next =
                                focused_to_mouse_x * focused_to_next_x +
                                focused_to_mouse_y * focused_to_next_y;
                            cos_mouse_to_prev /= focused_to_prev_norm;

                            i64 insert_index = focused_index;
                            if (cos_mouse_to_prev < cos_mouse_to_next)
                            {
                                insert_index = next_index;
                                focused_index = next_index;
                            }

                            control_points = realloc(control_points, (num_control_points + 1) * sizeof(ControlPoint));
                            for (i64 i = num_control_points - 1; i >= insert_index; i--)
                            {
                                control_points[i + 1] = control_points[i];
                            }

                            control_points[insert_index] = (ControlPoint)
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
                            focused_index = focused_index == 0 ? num_control_points : focused_index;
                            focused_index -= 1;
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

                    if (dist < 100)
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
        glyph_draw();
        render_end();
        printf("\x1b[2K\r%ldms", sdl_context.ticks - last_tick);
        fflush(stdout);
    }
exit:
    printf("\n");
    render_destroy();
    free(control_points);
    return 0;
}
