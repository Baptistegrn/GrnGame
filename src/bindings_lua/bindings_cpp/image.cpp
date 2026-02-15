/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* wrapper pour le type Sprite avec gestion memoire */
struct LuaSprite {
    Sprite *ptr;

    LuaSprite(const std::string &id, Sint16 width, Sint16 height) {
        ptr = createSprite(id.c_str(), width, height);
    }

    ~LuaSprite() {
        if (ptr) {
            freeSprite(ptr);
            ptr = nullptr;
        }
    }

    /* recupere/modifie les parametres */
    Sint16 rec_width() const { return ptr->taillex; }
    void mettre_width(Sint16 v) { ptr->taillex = v; }

    Sint16 rec_height() const { return ptr->tailley; }
    void mettre_height(Sint16 v) { ptr->tailley = v; }
};

/* dessine un rectangle vide */
void lua_draw_rect(float x, float y, Sint16 w, Sint16 h, sol::optional<Uint8> r,
                   sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawRect(x, y, w, h, red, green, blue, alpha, false);
}

/* dessine un rectangle plein */
void lua_draw_rect_filled(float x, float y, Sint16 w, Sint16 h, sol::optional<Uint8> r,
                          sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawRect(x, y, w, h, red, green, blue, alpha, true);
}

/* dessine une ligne entre deux points */
void lua_draw_line(float x1, float y1, float x2, float y2, sol::optional<Uint8> r,
                   sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawLine(x1, y1, x2, y2, red, green, blue, alpha);
}

/* dessine un cercle vide */
void lua_draw_circle(float x, float y, Sint16 radius, sol::optional<Uint8> r,
                     sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawCircle(x, y, radius, red, green, blue, alpha, false);
}

/* dessine un cercle plein */
void lua_draw_circle_filled(float x, float y, Sint16 radius, sol::optional<Uint8> r,
                            sol::optional<Uint8> g, sol::optional<Uint8> b,
                            sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawCircle(x, y, radius, red, green, blue, alpha, true);
}

/* dessine une image a la position indiquee */
void lua_draw(const std::string &path, float x, float y, Uint16 coeff, sol::optional<bool> flip,
              sol::optional<Uint16> rotationP, sol::optional<Uint16> rotation,
              sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    Uint16 rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    draw(path.c_str(), x, y, coeff, f, rotP, rot, alpha);
}

/* dessine du texte avec police personnalisee */
void lua_draw_text(const std::string &font_path, const std::string &text, float x, float y,
                   Uint16 scale, sol::optional<bool> flip, sol::optional<float> spacing,
                   sol::optional<Uint16> rotationP, sol::optional<Uint16> rotation,
                   sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    float sp = spacing.value_or(DEFAULT_SPACING);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint16 rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawText(font_path.c_str(), text.c_str(), x, y, scale, f, sp, rotP, rot, alpha);
}

/* definit l'icone de la fenetre */
void lua_set_icon(const std::string &path) { setIcon(path.c_str()); }

/* dessine un sprite a la position indiquee */
void lua_draw_sprite(const LuaSprite &sprite, Sint16 index, float x, float y, Sint16 coeff,
                     sol::optional<bool> flip, sol::optional<Uint16> rotation,
                     sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    drawSprite(sprite.ptr, index, x, y, coeff, f, rot, alpha);
}

/* charge toutes les images d'un dossier en memoire */
void lua_load_image_folder(const std::string &folder) { loadImageFolder(folder.c_str()); }

/* dessine un tableau de particules */
void lua_draw_particles(sol::table x_table, sol::table y_table, sol::table rotation_table,
                        sol::table a_table, sol::table r_table, sol::table g_table,
                        sol::table b_table) {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<Uint16> rotation;
    std::vector<Uint8> a;
    std::vector<Uint8> r;
    std::vector<Uint8> g;
    std::vector<Uint8> b;

    for (auto &kv : x_table)
        x.push_back(kv.second.as<float>());
    for (auto &kv : y_table)
        y.push_back(kv.second.as<float>());
    for (auto &kv : rotation_table)
        rotation.push_back(kv.second.as<Uint16>());
    for (auto &kv : a_table)
        a.push_back(kv.second.as<Uint8>());
    for (auto &kv : r_table)
        r.push_back(kv.second.as<Uint8>());
    for (auto &kv : g_table)
        g.push_back(kv.second.as<Uint8>());
    for (auto &kv : b_table)
        b.push_back(kv.second.as<Uint8>());

    int taille = x.size();
    /* de tableaux a pointeurs en c*/
    drawParticles(x.data(), y.data(), rotation.data(), a.data(), r.data(), g.data(), b.data(),
                  taille);
}

/* enregistrement des bindings image */
void enregistrer_bindings_image(sol::table &image) {
    /* Sprite */
    image.new_usertype<LuaSprite>(
        "Sprite", sol::call_constructor,
        sol::constructors<LuaSprite(const std::string &, Sint16, Sint16)>(), "width",
        sol::property(&LuaSprite::rec_width, &LuaSprite::mettre_width), "height",
        sol::property(&LuaSprite::rec_height, &LuaSprite::mettre_height));

    image.set_function("drawRect", &lua_draw_rect);
    image.set_function("drawRectFilled", &lua_draw_rect_filled);
    image.set_function("draw", &lua_draw);
    image.set_function("drawText", &lua_draw_text);
    image.set_function("setIcon", &lua_set_icon);
    image.set_function("drawSprite", &lua_draw_sprite);
    image.set_function("loadFolder", &lua_load_image_folder);
    image.set_function("drawParticles", &lua_draw_particles);
    image.set_function("freeFolder", &freeImageFolder);
    image.set_function("drawLine", &lua_draw_line);
    image.set_function("drawCircle", &lua_draw_circle);
    image.set_function("drawCircleFilled", &lua_draw_circle_filled);
    image.set_function("createSprite", &createSprite);
}