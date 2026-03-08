#include "input_manager.h"

InputManager InputManagerCreate()
{
    InputManager m = {
        .mouse = {.x = 0,
                  .y = 0,
                  .left_pressed = false,
                  .left_just_pressed = false,
                  .right_pressed = false,
                  .right_just_pressed = false,
                  .scroll_x = 0,
                  .scroll_y = 0},
        .drop_file = {0},
        .key_pressed = {false},
        .key_just_pressed = {false},
        .controllers = {0},
    };
    kv_init(m.text_input);
    return m;
}
