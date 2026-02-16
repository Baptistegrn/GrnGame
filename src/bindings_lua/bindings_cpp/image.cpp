/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
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

/* wrapper pour le type Sprite avec gestion memoire */
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

    /* recupere/modifie les parametres */
    Sint16 rec_largeur() const { return ptr->taillex; }
    void mettre_largeur(Sint16 v) { ptr->taillex = v; }

    Sint16 rec_hauteur() const { return ptr->tailley; }
    void mettre_hauteur(Sint16 v) { ptr->tailley = v; }
};

/* dessine un rectangle vide */
void lua_dessiner_rectangle(float x, float y, Sint16 w, Sint16 h, sol::optional<Uint8> r,
                            sol::optional<Uint8> g, sol::optional<Uint8> b,
                            sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_rectangle(x, y, w, h, red, green, blue, alpha, false);
}

/* dessine un rectangle plein */
void lua_dessiner_rectangle_plein(float x, float y, Sint16 w, Sint16 h, sol::optional<Uint8> r,
                                  sol::optional<Uint8> g, sol::optional<Uint8> b,
                                  sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_rectangle(x, y, w, h, red, green, blue, alpha, true);
}

/* dessine une ligne entre deux points */
void lua_dessiner_ligne(float x1, float y1, float x2, float y2, sol::optional<Uint8> r,
                        sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_ligne(x1, y1, x2, y2, red, green, blue, alpha);
}

/* dessine un cercle vide */
void lua_dessiner_cercle(float x, float y, Sint16 radius, sol::optional<Uint8> r,
                         sol::optional<Uint8> g, sol::optional<Uint8> b, sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_cercle(x, y, radius, red, green, blue, alpha, false);
}

/* dessine un cercle plein */
void lua_dessiner_cercle_plein(float x, float y, Sint16 radius, sol::optional<Uint8> r,
                               sol::optional<Uint8> g, sol::optional<Uint8> b,
                               sol::optional<Uint8> a) {
    Uint8 red = r.value_or(DEFAULT_R);
    Uint8 green = g.value_or(DEFAULT_G);
    Uint8 blue = b.value_or(DEFAULT_B);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_cercle(x, y, radius, red, green, blue, alpha, true);
}

/* dessine une image a la position indiquee */
void lua_dessiner_image(const std::string &path, float x, float y, Uint16 coeff,
                        sol::optional<bool> flip, sol::optional<Uint16> rotationP,
                        sol::optional<Uint16> rotation, sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    Uint16 rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_image_au_tableau(path.c_str(), x, y, coeff, f, rotP, rot, alpha);
}

/* dessine du texte avec police personnalisee */
void lua_dessiner_texte(const std::string &font_path, const std::string &text, float x, float y,
                        Uint16 scale, sol::optional<bool> flip, sol::optional<float> spacing,
                        sol::optional<Uint16> rotationP, sol::optional<Uint16> rotation,
                        sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    float sp = spacing.value_or(DEFAULT_SPACING);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint16 rotP = rotationP.value_or(DEFAULT_ROTATIONP);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_mot_dans_tableau(font_path.c_str(), text.c_str(), x, y, scale, f, sp, rotP, rot, alpha);
}

/* definit l'icone de la fenetre */
void lua_definir_icone(const std::string &path) { definir_icone(path.c_str()); }

/* dessine un sprite a la position indiquee */
void lua_dessiner_sprite(const LuaSprite &sprite, Sint16 index, float x, float y, Sint16 coeff,
                         sol::optional<bool> flip, sol::optional<Uint16> rotation,
                         sol::optional<Uint8> a) {
    bool f = flip.value_or(DEFAULT_FLIP);
    Uint16 rot = rotation.value_or(DEFAULT_ROTATION);
    Uint8 alpha = a.value_or(DEFAULT_ALPHA);
    ajouter_sprite_au_tableau(sprite.ptr, index, x, y, coeff, f, rot, alpha);
}

/* charge toutes les images d'un dossier en memoire */
void lua_charger_dossier_image(const std::string &folder) {
    charger_toutes_les_textures(folder.c_str());
}

/* dessine un tableau de particules */
void lua_dessiner_particules(sol::table x_table, sol::table y_table, sol::table rotation_table,
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
    ajouter_particules_au_tableau(x.data(), y.data(), rotation.data(), a.data(), r.data(), g.data(),
                                  b.data(), taille);
}

/* enregistrement des bindings image */
void enregistrer_bindings_image(sol::table &image) {
    /* Sprite */
    image.new_usertype<LuaSprite>(
        "Sprite", sol::call_constructor,
        sol::constructors<LuaSprite(const std::string &, Sint16, Sint16)>(), "width",
        sol::property(&LuaSprite::rec_largeur, &LuaSprite::mettre_largeur), "height",
        sol::property(&LuaSprite::rec_hauteur, &LuaSprite::mettre_hauteur));

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
    image.set_function("setKey",
                       [](int index, uint8_t valeur) { mettre_image_cle(index, valeur); });
    image.set_function("setIv", [](int index, uint8_t valeur) { mettre_image_iv(index, valeur); });
}