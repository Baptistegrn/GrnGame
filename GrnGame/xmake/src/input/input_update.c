#include "../main.h"
#include <SDL_image.h>
#include <string.h>

void input_update(Gestionnaire *jeu, GestionnaireEntrees *entrees) {
  if (!jeu || !entrees) {
    return;
  }

  SDL_Event event;

  entrees->mouse_just_pressed = false;
  entrees->mouse_right_just_pressed = false;
  memset(entrees->keys_pressed, false, sizeof(entrees->keys_pressed));
  memset(entrees->controller_pressed, false,
         sizeof(entrees->controller_pressed));

  while (SDL_PollEvent(&event)) {
    int raw_x, raw_y;
    SDL_GetMouseState(&raw_x, &raw_y);
    int coeff = jeu->largeur_actuel / jeu->largeur;

    entrees->mouse_x = (int)lroundf(((float)raw_x - (float)jeu->decalage_x) / (float)coeff);
    entrees->mouse_y = (int)lroundf(((float)raw_y - (float)jeu->decalage_y) / (float)coeff);

    switch (event.type) {
    case SDL_QUIT:
      jeu->run = false;
      break;

    case SDL_CONTROLLERBUTTONDOWN:
      if (event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX) {
        entrees->controller[event.cbutton.button] = true;
        entrees->controller_pressed[event.cbutton.button] = true;
      }
      break;

    case SDL_CONTROLLERBUTTONUP:
      if (event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX) {
        entrees->controller[event.cbutton.button] = false;
      }
      break;

    case SDL_CONTROLLERAXISMOTION:
      switch (event.caxis.axis) {
      case SDL_CONTROLLER_AXIS_LEFTX:
        entrees->Joy.left.x = event.caxis.value;
        break;
      case SDL_CONTROLLER_AXIS_LEFTY:
        entrees->Joy.left.y = event.caxis.value;
        break;
      case SDL_CONTROLLER_AXIS_RIGHTX:
        entrees->Joy.right.x = event.caxis.value;
        break;
      case SDL_CONTROLLER_AXIS_RIGHTY:
        entrees->Joy.right.y = event.caxis.value;
        break;
      case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
        entrees->trigger.triggerleft = event.caxis.value;
        break;
      case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
        entrees->trigger.triggerright = event.caxis.value;
        break;
      }
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        entrees->mouse_pressed = true;
        entrees->mouse_just_pressed = true;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        entrees->mouse_right_pressed = true;
        entrees->mouse_right_just_pressed = true;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      if (event.button.button == SDL_BUTTON_LEFT) {
        entrees->mouse_pressed = false;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        entrees->mouse_right_pressed = false;
      }
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
        entrees->keys[event.key.keysym.scancode] = true;
        entrees->keys_pressed[event.key.keysym.scancode] = true;
      }
      break;

    case SDL_KEYUP:
      if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
        entrees->keys[event.key.keysym.scancode] = false;
      }
      break;

    default:
      break;
    }
  }
}