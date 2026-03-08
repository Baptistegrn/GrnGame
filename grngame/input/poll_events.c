#include "../core/app.h"
#include "cglm/types-struct.h"
#include "controller.h"
#include "grngame/core/window.h"
#include "grngame/utils/string_compat.h"
#include "mouse.h"
#include <SDL3/SDL_events.h>

static int16 FindControllerIndex(SDL_JoystickID id)
{
    SDL_Gamepad *gp = GamepadFromID(id);
    if (!gp)
        return -1;
    for (int16 i = 0; i < MAX_CONTROLLERS; i++)
    {
        if (g_app.input_manager.controllers[i].gamepad == gp)
            return i;
    }
    return -1;
}

static void ResetInputManagerKeys()
{
    InputManager *im = &g_app.input_manager;

    im->mouse.left_just_pressed = false;
    im->mouse.right_just_pressed = false;
    im->mouse.scroll_x = 0;
    im->mouse.scroll_y = 0;
    memset(im->key_just_pressed, 0, sizeof(im->key_just_pressed));
    for (int i = 0; i < MAX_CONTROLLERS; i++)
        memset(im->controllers[i].just_pressed, 0, sizeof(im->controllers[i].just_pressed));
}

static void UpdateMousePosition()
{
    vec2s i = GetMousePosition();
    uint8 coeff = WidndowGetScale();
    g_app.input_manager.mouse.x = (int32)SDL_roundf(i.x / (float32)coeff - g_app.info.offset_x);
    g_app.input_manager.mouse.y = (int32)SDL_roundf(i.y / (float32)coeff - g_app.info.offset_y);
}

static void AppendTextInput(const char *text)
{
    InputManager *im = &g_app.input_manager;
    while (*text)
    {
        kv_push(char, im->text_input, *text);
        text++;
    }
}

static void BackspaceTextInput()
{
    InputManager *im = &g_app.input_manager;
    if (kv_size(im->text_input) > 0)
        kv_size(im->text_input)--;
}

void PollEvents()
{
    InputManager *im = &g_app.input_manager;
    ResetInputManagerKeys();
    UpdateMousePosition();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            EngineStop();
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            ApplyResizing(&g_app.info, (int16)event.window.data1, (int16)event.window.data2);
            break;

        case SDL_EVENT_WINDOW_MAXIMIZED: {
            ivec2s i = WindowDimensions(g_app.window);
            ApplyResizing(&g_app.info, (int16)i.x, (int16)i.y);
            break;
        }
        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat)
            {
                if (event.key.scancode == SDL_SCANCODE_BACKSPACE)
                    BackspaceTextInput();

                if (event.key.scancode < SDL_SCANCODE_COUNT)
                {
                    im->key_pressed[event.key.scancode] = true;
                    im->key_just_pressed[event.key.scancode] = true;
                }
            }
            break;

        case SDL_EVENT_KEY_UP:
            if (event.key.scancode < SDL_SCANCODE_COUNT)
                im->key_pressed[event.key.scancode] = false;
            break;
        case SDL_EVENT_TEXT_INPUT:
            AppendTextInput(event.text.text);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                im->mouse.left_pressed = true;
                im->mouse.left_just_pressed = true;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                im->mouse.right_pressed = true;
                im->mouse.right_just_pressed = true;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT)
                im->mouse.left_pressed = false;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                im->mouse.right_pressed = false;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            if (event.wheel.y > 0)
                im->mouse.scroll_y = 1;
            else if (event.wheel.y < 0)
                im->mouse.scroll_y = -1;
            if (event.wheel.x > 0)
                im->mouse.scroll_x = 1;
            else if (event.wheel.x < 0)
                im->mouse.scroll_x = -1;
            break;
        case SDL_EVENT_DROP_FILE:
            if (event.drop.data)
            {
                strncpy_s(im->drop_file, DROP_FILE_PATH_MAX, event.drop.data, DROP_FILE_PATH_MAX - 1);
                im->drop_file[DROP_FILE_PATH_MAX - 1] = '\0';
            }
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
            int16 idx = FindControllerIndex(event.gbutton.which);
            if (idx >= 0 && event.gbutton.button < SDL_GAMEPAD_BUTTON_COUNT)
            {
                im->controllers[idx].pressed[event.gbutton.button] = true;
                im->controllers[idx].just_pressed[event.gbutton.button] = true;
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_UP: {
            int16 idx = FindControllerIndex(event.gbutton.which);
            if (idx >= 0 && event.gbutton.button < SDL_GAMEPAD_BUTTON_COUNT)
                im->controllers[idx].pressed[event.gbutton.button] = false;
            break;
        }

        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            int16 idx = FindControllerIndex(event.gaxis.which);
            if (idx < 0)
                break;
            switch (event.gaxis.axis)
            {
            case SDL_GAMEPAD_AXIS_LEFTX:
                im->controllers[idx].stick_lx = event.gaxis.value;
                break;
            case SDL_GAMEPAD_AXIS_LEFTY:
                im->controllers[idx].stick_ly = event.gaxis.value;
                break;
            case SDL_GAMEPAD_AXIS_RIGHTX:
                im->controllers[idx].stick_rx = event.gaxis.value;
                break;
            case SDL_GAMEPAD_AXIS_RIGHTY:
                im->controllers[idx].stick_ry = event.gaxis.value;
                break;
            case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
                im->controllers[idx].trigger_l = event.gaxis.value;
                break;
            case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
                im->controllers[idx].trigger_r = event.gaxis.value;
                break;
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_ADDED:
            ControllerOpen((int16)event.gdevice.which);
            break;

        case SDL_EVENT_GAMEPAD_REMOVED: {
            int16 idx = FindControllerIndex(event.gdevice.which);
            if (idx >= 0)
                ControllerClose((int16)idx);
            break;
        }

        default:
            break;
        }
    }
}