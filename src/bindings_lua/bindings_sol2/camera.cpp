/*
 * Utilitaires Lua et wrapper C++ pour la camera .
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include <sol/sol.hpp>

extern "C" {
#include "../../main.h"
#include "../../module_jeu/camera/camera.h"
#include "../../module_jeu/module_jeu.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* fonctions de la camera */
void lua_update_camera(float tx, float ty, float dt) {
    if (gs && gs->module_jeu->camera)
        camera_mise_a_jour(gs->module_jeu->camera, tx, ty, dt);
}

void lua_shake_camera(float intensite, float duree, float decay) {
    if (gs && gs->module_jeu->camera)
        camera_trembler(gs->module_jeu->camera, intensite, duree, decay);
}

/* position */
float lua_get_cam_x() { return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->x : 0; }
void lua_set_cam_x(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->x = v;
}
float lua_get_cam_y() { return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->y : 0; }
void lua_set_cam_y(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->y = v;
}

/* lissage */
float lua_get_cam_smooth() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->facteur_lissage : 0;
}
void lua_set_cam_smooth(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->facteur_lissage = v;
}

/* dimensions */
int lua_get_cam_w() { return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->w : 0; }
void lua_set_cam_w(int v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->w = v;
}
int lua_get_cam_h() { return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->h : 0; }
void lua_set_cam_h(int v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->h = v;
}

/* limites */
float lua_get_cam_limite_gauche() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->limite_gauche : 0.0f;
}
void lua_set_cam_limite_gauche(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->limite_gauche = v;
}
float lua_get_cam_limite_droite() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->limite_droite : 0.0f;
}
void lua_set_cam_limite_droite(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->limite_droite = v;
}
float lua_get_cam_limite_haut() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->limite_haut : 0.0f;
}
void lua_set_cam_limite_haut(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->limite_haut = v;
}
float lua_get_cam_limite_bas() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->limite_bas : 0.0f;
}
void lua_set_cam_limite_bas(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->limite_bas = v;
}

/* tremblement */
float lua_get_cam_shake_intensite() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->tremblement_intensite : 0.0f;
}
void lua_set_cam_shake_intensite(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->tremblement_intensite = v;
}
float lua_get_cam_shake_duree() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->tremblement_duree : 0.0f;
}
void lua_set_cam_shake_duree(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->tremblement_duree = v;
}
float lua_get_cam_shake_decay() {
    return (gs && gs->module_jeu->camera) ? gs->module_jeu->camera->tremblement_decay : 0.0f;
}
void lua_set_cam_shake_decay(float v) {
    if (gs && gs->module_jeu->camera)
        gs->module_jeu->camera->tremblement_decay = v;
}

/* enregistre les bindings de camera */
void enregistrer_bindings_camera(sol::table &camera) {
    camera.set_function("createCamera", &creer_camera);
    camera.set_function("updateCamera", &lua_update_camera);
    camera.set_function("shakeCamera", &lua_shake_camera);

    /* position */
    camera.set_function("setCameraX", &lua_set_cam_x);
    camera.set_function("getCameraX", &lua_get_cam_x);
    camera.set_function("setCameraY", &lua_set_cam_y);
    camera.set_function("getCameraY", &lua_get_cam_y);

    /* lissage */
    camera.set_function("setCameraSmooth", &lua_set_cam_smooth);
    camera.set_function("getCameraSmooth", &lua_get_cam_smooth);

    /* dimensions */
    camera.set_function("setCameraW", &lua_set_cam_w);
    camera.set_function("getCameraW", &lua_get_cam_w);
    camera.set_function("setCameraH", &lua_set_cam_h);
    camera.set_function("getCameraH", &lua_get_cam_h);

    /* limites */
    camera.set_function("setCameraLimitLeft", &lua_set_cam_limite_gauche);
    camera.set_function("getCameraLimitLeft", &lua_get_cam_limite_gauche);
    camera.set_function("setCameraLimitRight", &lua_set_cam_limite_droite);
    camera.set_function("getCameraLimitRight", &lua_get_cam_limite_droite);
    camera.set_function("setCameraLimitHeight", &lua_set_cam_limite_haut);
    camera.set_function("getCameraLimitHeight", &lua_get_cam_limite_haut);
    camera.set_function("setCameraLimitDown", &lua_set_cam_limite_bas);
    camera.set_function("getCameraLimiteDown", &lua_get_cam_limite_bas);
}