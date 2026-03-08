#include "../core/app.h"
#include "grngame/dev/logging.h"

const char *InputTextGet()
{
    InputManager *im = &g_app.input_manager;
    if (kv_size(im->text_input) == 0)
    {
        LOG_INFO("Input text is empty");
        return "";
    }
    kv_push(char, im->text_input, '\0');
    kv_size(im->text_input)--;
    return im->text_input.a;
}

void InputTextClear()
{
    kv_size(g_app.input_manager.text_input) = 0;
    LOG_INFO("text clear");
}

const char *InputDropFile()
{
    return g_app.input_manager.drop_file;
}