/*
 * Enregistrement des bindings GrnGame dans la VM Lua pour l'affichage.
 */

#include "SDL_stdinc.h"
#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../main.h"
#include "../../moteur/fenetre/fenetre.h"
#include "../../moteur/image/affichage/affichage.h"
#include "../../moteur/image/ajout/ajout.h"
#include "../../moteur/image/caractere/caractere.h"
#include "../../moteur/image/chargement/chargement_image.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

struct LuaSprite {
    Sprite *ptr;

    LuaSprite(const std::string &id, Sint16 width, Sint16 height) {
        ptr = creer_sprite(id.c_str(), width, height);
    }

    ~LuaSprite() {
        if (ptr) {
            liberer_sprite(ptr);
            ptr = nullptr;
        }
    }

    Sint16 rec_largeur() const { return ptr->taillex; }
    void mettre_largeur(Sint16 v) { ptr->taillex = v; }

    Sint16 rec_hauteur() const { return ptr->tailley; }
    void mettre_hauteur(Sint16 v) { ptr->tailley = v; }
};

struct LuaParticle {
    Particule *ptr;

    LuaParticle(float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint16 rotation,
                sol::optional<Uint8> a) {
        Uint8 alpha = a.value_or(DEFAULT_ALPHA);
        ptr = creer_particule(x, y, r, g, b, alpha, rotation);
    }

    ~LuaParticle() {
        if (ptr) {
            liberer_particule(ptr);
            ptr = nullptr;
        }
    }

    float get_x() const { return ptr->x; }
    void set_x(float v) { ptr->x = v; }

    float get_y() const { return ptr->y; }
    void set_y(float v) { ptr->y = v; }

    Uint16 get_rotation() const { return ptr->rotation; }
    void set_rotation(Uint16 v) { ptr->rotation = v; }

    Uint8 get_a() const { return ptr->a; }
    void set_a(Uint8 v) { ptr->a = v; }

    Uint8 get_r() const { return ptr->r; }
    void set_r(Uint8 v) { ptr->r = v; }

    Uint8 get_g() const { return ptr->g; }
    void set_g(Uint8 v) { ptr->g = v; }

    Uint8 get_b() const { return ptr->b; }
    void set_b(Uint8 v) { ptr->b = v; }
};

struct LuaParticles {
    Particules *ptr;

    LuaParticles() { ptr = creer_particules(); }

    ~LuaParticles() {
        if (ptr) {
            liberer_particules(ptr);
            ptr = nullptr;
        }
    }

    void add(const LuaParticle &particle) { ajouter_particule(ptr, particle.ptr); }
    int size() const { return ptr->taille; }
    Particule *get(int index) const { return &ptr->tab[index]; }
    Particule &operator[](int index) { return ptr->tab[index]; }

    std::function<Particule *()> pairs() {
        int index = -1;
        int max_size = ptr->taille;
        Particules *parts_ptr = ptr;

        return [parts_ptr, index, max_size]() mutable -> Particule * {
            index++;
            if (index < max_size) {
                return &parts_ptr->tab[index];
            }
            return nullptr;
        };
    }
};

void lua_dessiner_rectangle(float x, float y, Sint16 w, Sint16 h, sol::optional<Uint8> r,
                            sol::optional<Uint8> g, sol::optional<Uint8> b,
                            sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    if (gs)
        ajouter_rectangle(x, y, w, h, red, green, blue, alpha, false);
}

void lua_dessiner_rectangle_plein(float x, float y, Sint16 w, Sint16 h, sol::optional<Uint8> r,
                                  sol::optional<Uint8> g, sol::optional<Uint8> b,
                                  sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);

    ajouter_rectangle(x, y, w, h, red, green, blue, alpha, true);
}

void lua_dessiner_ligne(float x1, float y1, float x2, float y2, sol::optional<Uint8> r,
                        sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_ligne(x1, y1, x2, y2, red, green, blue, alpha);
}

void lua_dessiner_cercle(float x, float y, Sint16 radius, sol::optional<Uint8> r,
                         sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_cercle(x, y, radius, red, green, blue, alpha, false);
}

void lua_dessiner_cercle_plein(float x, float y, Sint16 radius, sol::optional<Uint8> r,
                               sol::optional<Uint8> g, sol::optional<Uint8> b,
                               sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_cercle(x, y, radius, red, green, blue, alpha, true);
}

void lua_dessiner_image(const std::string &path, float x, float y, Uint16 coeff,
                        sol::optional<bool> flip, sol::optional<Uint16> rotationP,
                        sol::optional<Uint16> rotation, sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    Uint16 rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_image_au_tableau(path.c_str(), x, y, coeff, f, rotP, rot, alpha);
}

float lua_dessiner_texte(const std::string &font_path, const std::string &text, float x, float y,
                         Uint16 scale, sol::optional<bool> flip, sol::optional<float> spacing,
                         sol::optional<Uint16> rotationP, sol::optional<Uint16> rotation,
                         sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    float sp = spacing.value_or(DEFAULT_SPACING);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint16 rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    return ajouter_mot_dans_tableau(font_path.c_str(), text.c_str(), x, y, scale, f, sp, rotP, rot,
                                    alpha);
}

void lua_definir_icone(const std::string &path) { definir_icone(path.c_str()); }

void lua_dessiner_sprite(const LuaSprite &sprite, Sint16 index, float x, float y, Sint16 coeff,
                         sol::optional<bool> flip, sol::optional<Uint16> rotation,
                         sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_sprite_au_tableau(sprite.ptr, index, x, y, coeff, f, rot, alpha);
}

void lua_charger_dossier_image(const std::string &folder) {
    charger_toutes_les_textures(folder.c_str());
}

void lua_dessiner_particules(LuaParticles &particules) {
    ajouter_particules_au_tableau(particules.ptr);
}

void enregistrer_bindings_image(sol::table &image) {
    image.new_usertype<LuaSprite>(
        "Sprite", sol::call_constructor,
        sol::constructors<LuaSprite(const std::string &, Sint16, Sint16)>(), "width",
        sol::property(&LuaSprite::rec_largeur, &LuaSprite::mettre_largeur), "height",
        sol::property(&LuaSprite::rec_hauteur, &LuaSprite::mettre_hauteur));

    image.new_usertype<Particule>("ParticleRaw", sol::no_constructor, "x", &Particule::x, "y",
                                  &Particule::y, "rotation", &Particule::rotation, "a",
                                  &Particule::a, "r", &Particule::r, "g", &Particule::g, "b",
                                  &Particule::b);

    image.new_usertype<LuaParticle>(
        "Particle", sol::call_constructor,
        sol::constructors<LuaParticle(float, float, Uint8, Uint8, Uint8, Uint16,
                                      sol::optional<Uint8>)>(),
        "x", sol::property(&LuaParticle::get_x, &LuaParticle::set_x), "y",
        sol::property(&LuaParticle::get_y, &LuaParticle::set_y), "rotation",
        sol::property(&LuaParticle::get_rotation, &LuaParticle::set_rotation), "a",
        sol::property(&LuaParticle::get_a, &LuaParticle::set_a), "r",
        sol::property(&LuaParticle::get_r, &LuaParticle::set_r), "g",
        sol::property(&LuaParticle::get_g, &LuaParticle::set_g), "b",
        sol::property(&LuaParticle::get_b, &LuaParticle::set_b));

    image.new_usertype<LuaParticles>(
        "Particles", sol::call_constructor, sol::constructors<LuaParticles()>(), "add",
        &LuaParticles::add, "size", &LuaParticles::size, "get", &LuaParticles::get,
        sol::meta_function::index, &LuaParticles::operator[], "pairs", &LuaParticles::pairs);

    image.set_function("drawRect", &lua_dessiner_rectangle);
    image.set_function("drawRectFilled", &lua_dessiner_rectangle_plein);
    image.set_function("draw", &lua_dessiner_image);
    image.set_function("drawText", &lua_dessiner_texte);
    image.set_function("setIcon", &lua_definir_icone);
    image.set_function("drawSprite", &lua_dessiner_sprite);
    image.set_function("loadFolder", &lua_charger_dossier_image);
    image.set_function("drawParticles", &lua_dessiner_particules);
    image.set_function("freeFolder", &liberer_gestionnaire_image);
    image.set_function("drawLine", &lua_dessiner_ligne);
    image.set_function("drawCircle", &lua_dessiner_cercle);
    image.set_function("drawCircleFilled", &lua_dessiner_cercle_plein);
    image.set_function("cls", &stocker_coloriage);
    image.set_function("setKey",
                       [](int index, uint8_t valeur) { mettre_image_cle(index, valeur); });
    image.set_function("setIv", [](int index, uint8_t valeur) { mettre_image_iv(index, valeur); });
}