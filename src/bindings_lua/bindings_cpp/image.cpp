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

/* classe sprite */
struct LuaSprite {
    Sprite *ptr;

    LuaSprite(const std::string &id, int width, int height) {
        ptr = createSprite(id.c_str(), width, height);
    }

    ~LuaSprite() {
        if (ptr) {
            freeSprite(ptr);
            ptr = nullptr;
        }
    }

    /* pour modifier / recuperer les parametres */
    int rec_width() const { return ptr->taillex; }
    void mettre_width(int v) { ptr->taillex = v; }

    int rec_height() const { return ptr->tailley; }
    void mettre_height(int v) { ptr->tailley = v; }
};

/* dessiner rect */
void lua_draw_rect(float x, float y, float w, float h, sol::optional<int> r, sol::optional<int> g,
                   sol::optional<int> b, sol::optional<int> a) {
    int red = r.value_or(DEFAULT_R);
    int green = g.value_or(DEFAULT_G);
    int blue = b.value_or(DEFAULT_B);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawRect(x, y, w, h, red, green, blue, alpha, true);
}

/* dessiner rect plein */
void lua_draw_rect_filled(float x, float y, float w, float h, sol::optional<int> r,
                          sol::optional<int> g, sol::optional<int> b, sol::optional<int> a) {
    int red = r.value_or(DEFAULT_R);
    int green = g.value_or(DEFAULT_G);
    int blue = b.value_or(DEFAULT_B);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawRect(x, y, w, h, red, green, blue, alpha, false);
}

/* dessiner une ligne */
void lua_draw_line(float x1, float y1, float x2, float y2, sol::optional<int> r,
                   sol::optional<int> g, sol::optional<int> b, sol::optional<int> a) {
    int red = r.value_or(DEFAULT_R);
    int green = g.value_or(DEFAULT_G);
    int blue = b.value_or(DEFAULT_B);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawLine(x1, y1, x2, y2, red, green, blue, alpha);
}

/* dessine un cercle vide */
void lua_draw_circle(float x, float y, float radius, sol::optional<int> r, sol::optional<int> g,
                     sol::optional<int> b, sol::optional<int> a) {
    int red = r.value_or(DEFAULT_R);
    int green = g.value_or(DEFAULT_G);
    int blue = b.value_or(DEFAULT_B);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawCircle(x, y, radius, red, green, blue, alpha, false);
}

/* dessine un cercle plein */
void lua_draw_circle_filled(float x, float y, float radius, sol::optional<int> r,
                            sol::optional<int> g, sol::optional<int> b, sol::optional<int> a) {
    int red = r.value_or(DEFAULT_R);
    int green = g.value_or(DEFAULT_G);
    int blue = b.value_or(DEFAULT_B);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawCircle(x, y, radius, red, green, blue, alpha, true);
}

void lua_draw(const std::string &path, float x, float y, float w, float h, sol::optional<bool> flip,
              sol::optional<float> rotationP, sol::optional<float> rotation, sol::optional<int> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    float rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    float rot = rotation.value_or(DEFAULT_ROTATION);
    int alpha = a.value_or(DEFAULT_ALPHA);
    draw(path.c_str(), x, y, w, h, f, rotP, rot, alpha);
}

/* dessiner texte */
void lua_draw_text(const std::string &font_path, const std::string &text, float x, float y,
                   float scale, sol::optional<bool> flip, sol::optional<float> spacing,
                   sol::optional<int> rotationP, sol::optional<int> rotation,
                   sol::optional<int> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    float sp = spacing.value_or(DEFAULT_SPACING);
    int rot = rotation.value_or(DEFAULT_ROTATION);
    int rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawText(font_path.c_str(), text.c_str(), x, y, scale, f, sp, rotP, rot, alpha);
}

/* definir l'icone de la fenetre */
void lua_set_icon(const std::string &path) { setIcon(path.c_str()); }

/* dessiner un sprite */
void lua_draw_sprite(const LuaSprite &sprite, int index, float x, float y, float w, float h,
                     sol::optional<bool> flip, sol::optional<int> rotation, sol::optional<int> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    int rot = rotation.value_or(DEFAULT_ROTATION);
    int alpha = a.value_or(DEFAULT_ALPHA);
    drawSprite(sprite.ptr, index, x, y, w, h, f, rot, alpha);
}

/* charger un dossier d'images */
void lua_load_image_folder(const std::string &folder) { loadImageFolder(folder.c_str()); }

/* dessine des particules */
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

/* enregistrer les bindings de image */
void enregistrer_bindings_image(sol::table &image) {
    /* Sprite */
    image.new_usertype<LuaSprite>(
        "Sprite", sol::call_constructor,
        sol::constructors<LuaSprite(const std::string &, int, int)>(), "width",
        sol::property(&LuaSprite::rec_width, &LuaSprite::mettre_width), "height",
        sol::property(&LuaSprite::rec_height, &LuaSprite::mettre_height));

    image.set_function("drawRect", &lua_draw_rect);
    image.set_function("drawRectFilled", &lua_draw_rect_filled);
    image.set_function("draw", &lua_draw);
    image.set_function("drawText", &lua_draw_text);
    image.set_function("setIcon", &lua_set_icon);
    image.set_function("drawSprite", &lua_draw_sprite);
    image.set_function("loadImageFolder", &lua_load_image_folder);
    image.set_function("drawParticles", &lua_draw_particles);
    image.set_function("freeImageFolder", &freeImageFolder);
    image.set_function("drawLine", &lua_draw_line);
    image.set_function("drawCircle", &lua_draw_circle);
    image.set_function("drawCircleFilled", &lua_draw_circle_filled);
    image.set_function("createSprite", &createSprite);
}