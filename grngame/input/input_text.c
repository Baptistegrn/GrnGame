#include "../core/app.h"

const char *InputTextGet()
{
    InputManager *im = &g_app.input_manager;
    if (kv_size(im->text_input) == 0)
    {
        // todo log
        return "";
    }
    kv_push(char, im->text_input, '\0');
    kv_size(im->text_input)--;
    return im->text_input.a;
}

void InputTextClear()
{
    kv_size(g_app.input_manager.text_input) = 0;
    // todo log
}

const char *InputDropFile()
{
    return g_app.input_manager.drop_file;
}