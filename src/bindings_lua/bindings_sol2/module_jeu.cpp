/*
 * Utilitaires Lua et wrapper C++ pour les modules de jeu.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>
#include <vector>

extern "C" {
#include "../../main.h"
#include "../../module_jeu/blocs/bloc.h"
#include "../../module_jeu/camera/camera.h"
#include "../../module_jeu/carte/carte.h"
#include "../../module_jeu/hitbox/hitbox.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* wrapper pour le type Block avec gestion memoire */
struct LuaBlock {
    Block *ptr;

    LuaBlock(float x, float y, float w, float h, int type) { ptr = creer_block(x, y, w, h, type); }

    ~LuaBlock() {
        if (ptr) {
            liberer_block(ptr);
            ptr = nullptr;
        }
    }

    /* recupere/modifie les parametres */
    float rec_x() const { return ptr->x; }
    void mettre_x(float v) { ptr->x = v; }

    float rec_y() const { return ptr->y; }
    void mettre_y(float v) { ptr->y = v; }

    float rec_w() const { return ptr->w; }
    void mettre_w(float v) { ptr->w = v; }

    float rec_h() const { return ptr->h; }
    void mettre_h(float v) { ptr->h = v; }

    int rec_type() const { return ptr->type; }
    void mettre_type(int t) { ptr->type = t; }
};

/* wrapper pour le conteneur de blocs */
struct LuaBlocks {
    Blocks *ptr;

    LuaBlocks() { ptr = creer_blocks(); }

    ~LuaBlocks() {
        if (ptr) {
            liberer_blocks(ptr);
            ptr = nullptr;
        }
    }

    /* ajoute un bloc au tableau */
    void add(const LuaBlock &block) { ajouter_block(ptr, block.ptr); }

    /* recupere la taille du tableau */
    int size() const { return taille_blocks(ptr); }

    /* recupere un bloc par son index */
    Block *get(int index) const { return &ptr->tab[index]; }

    /* acces par indice [] */
    Block &operator[](int index) { return ptr->tab[index]; }

    /* itrateur pour for...in */
    std::function<Block *()> pairs() {
        int index = -1;
        int max_size = taille_blocks(ptr);
        Blocks *blocks_ptr = ptr;

        return [blocks_ptr, index, max_size]() mutable -> Block * {
            index++;
            if (index < max_size) {
                return &blocks_ptr->tab[index];
            }
            return nullptr;
        };
    }
};

/* wrapper pour l'entite topdown avec gestion memoire */
struct LuaEntityTopdown {
    EntityTopdown *ptr;

    LuaEntityTopdown(float x, float y, float w, float h) { ptr = creer_entite_topdown(x, y, w, h); }

    ~LuaEntityTopdown() {
        if (ptr) {
            liberer_entite_topdown(ptr);
            ptr = nullptr;
        }
    }

    /* recupere/modifie les parametres */
    float rec_x() const { return ptr->x; }
    void mettre_x(float v) { ptr->x = v; }

    float rec_y() const { return ptr->y; }
    void mettre_y(float v) { ptr->y = v; }

    float rec_w() const { return ptr->width; }
    void mettre_w(float v) { ptr->width = v; }

    float rec_h() const { return ptr->height; }
    void mettre_h(float v) { ptr->height = v; }
};

/* wrapper pour l'entite platformer avec gestion memoire */
struct LuaEntityPlatformer {
    EntityPlatformer *ptr;

    LuaEntityPlatformer(float x, float y, float w, float h, sol::optional<float> force_saut,
                        sol::optional<float> gravite) {
        float f = force_saut.value_or(DEFAULT_FORCE_SAUT);
        float g = gravite.value_or(DEFAULT_GRAVITE);
        ptr = creer_entite_platforme(x, y, w, h, f, g);
    }

    ~LuaEntityPlatformer() {
        if (ptr) {
            liberer_entite_platforme(ptr);
            ptr = nullptr;
        }
    }

    /* recupere/modifie les parametres */
    float rec_x() const { return ptr->x; }
    void mettre_x(float v) { ptr->x = v; }

    float rec_y() const { return ptr->y; }
    void mettre_y(float v) { ptr->y = v; }

    float rec_w() const { return ptr->width; }
    void mettre_w(float v) { ptr->width = v; }

    float rec_h() const { return ptr->height; }
    void mettre_h(float v) { ptr->height = v; }

    float rec_force_saut() const { return ptr->powerJump; }
    void mettre_force_saut(float v) { ptr->powerJump = v; }

    float rec_gravite() const { return ptr->gravity; }
    void mettre_gravite(float v) { ptr->gravity = v; }

    bool rec_demande_saut() const { return ptr->requestJump; }
    void mettre_demande_saut(bool v) { ptr->requestJump = v; }

    bool rec_verrou_gauche() const { return ptr->leftLock; }
    void mettre_verrou_gauche(bool v) { ptr->leftLock = v; }

    bool rec_verrou_droit() const { return ptr->rightLock; }
    void mettre_verrou_droit(bool v) { ptr->rightLock = v; }

    bool rec_en_air() const { return ptr->inSky; }
    void mettre_en_air(bool v) { ptr->inSky = v; }

    float rec_vitesseY() const { return ptr->speedY; }
    void mettre_vitesseY(float v) { ptr->speedY = v; }
};

/* met a jour la camera globale du moteur */
void lua_update_camera(float tx, float ty, float dt) {
    if (gs && gs->camera) {
        camera_mise_a_jour(gs->camera, tx, ty, dt);
    }
}

/* recupere la position X de la camera */
float lua_get_cam_x() { return (gs && gs->camera) ? gs->camera->x : 0; }

/* definit la position X de la camera */
void lua_set_cam_x(float v) {
    if (gs && gs->camera)
        gs->camera->x = v;
}

/* recupere la position Y de la camera */
float lua_get_cam_y() { return (gs && gs->camera) ? gs->camera->y : 0; }

/* definit la position Y de la camera */
void lua_set_cam_y(float v) {
    if (gs && gs->camera)
        gs->camera->y = v;
}

/* recupere le lissage de la camera */
float lua_get_cam_smooth() { return (gs && gs->camera) ? gs->camera->smooth_factor : 0; }

/* definit le lissage de la camera */
void lua_set_cam_smooth(float v) {
    if (gs && gs->camera)
        gs->camera->smooth_factor = v;
}

/* wrapper pour les blocs charges depuis un fichier */
struct LuaBlocksFromFile {
    Blocks *ptr;
    bool owns;

    LuaBlocksFromFile(const std::string &path, int pas_x, int pas_y, char sep, int exclude) {
        ptr = charger_carte(path.c_str(), pas_x, pas_y, sep, exclude);
        owns = true;
    }

    ~LuaBlocksFromFile() {
        if (ptr && owns) {
            liberer_blocks(ptr);
            ptr = nullptr;
        }
    }

    int size() const { return taille_blocks(ptr); }
};

/* convertit un tableau lua de blocs en structure Blocks */
static Blocks *blocks_depuis_tableau(sol::table t) {
    Blocks *blocks = creer_blocks();

    for (auto &kv : t) {
        LuaBlock &b = kv.second.as<LuaBlock &>();
        ajouter_block(blocks, b.ptr);
    }
    return blocks;
}

/* calcule la physique de plateforme avec collisions */
static void lua_hitbox_platformer(LuaEntityPlatformer &ent, LuaBlocks &blocks,
                                  sol::optional<float> vmax, sol::optional<float> correction,
                                  sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }
    float c = correction.value_or(DEFAULT_CORRECTION_MUR);
    float v = vmax.value_or(DEFAULT_VMAX_CHUTE);
    hitbox_platforme(ent.ptr, blocks.ptr, v, c, ig.data(), ig.size());
}

/* calcule la physique de plateforme avec table lua */
static EntityPlatformer *lua_hitbox_platformer_table(LuaEntityPlatformer &ent, sol::table t,
                                                     sol::optional<float> vmax,
                                                     sol::optional<float> correction,
                                                     sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }

    Blocks *blocks = blocks_depuis_tableau(t);
    float c = correction.value_or(DEFAULT_CORRECTION_MUR);
    float v = vmax.value_or(DEFAULT_VMAX_CHUTE);
    EntityPlatformer *r = hitbox_platforme(ent.ptr, blocks, v, c, ig.data(), ig.size());

    liberer_blocks(blocks);
    return r;
}

/* calcule la physique topdown avec collisions */
static EntityTopdown *lua_hitbox_topdown(LuaEntityTopdown &ent, LuaBlocks &blocks,
                                         sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }
    return hitbox_topdown(ent.ptr, blocks.ptr, ig.data(), ig.size());
}

/* calcule la physique topdown avec table lua */
static EntityTopdown *lua_hitbox_topdown_table(LuaEntityTopdown &ent, sol::table t,
                                               sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }

    Blocks *blocks = blocks_depuis_tableau(t);
    EntityTopdown *r = hitbox_topdown(ent.ptr, blocks, ig.data(), ig.size());
    liberer_blocks(blocks);
    return r;
}

/* fonction damination  */
std::tuple<int, float> lua_animation(int frame, float timer, int rangeStart, int rangeEnd,
                                     float speed, float dt, bool loop) {
    timer += dt;
    if (timer >= speed) {
        timer = 0.0f;
        frame++;
        if (frame > rangeEnd) {
            frame = loop ? rangeStart : rangeEnd;
        }
    }
    return std::make_tuple(frame, timer);
}

/* enregistrement des bindings module jeu */
void enregistrer_bindings_module_jeu(sol::table &game) {
    game.new_usertype<Block>("BlockRaw", sol::no_constructor, "x", &Block::x, "y", &Block::y, "w",
                             &Block::w, "h", &Block::h, "type", &Block::type);

    /* Block */
    game.new_usertype<LuaBlock>("Block", sol::call_constructor,
                                sol::constructors<LuaBlock(float, float, float, float, int)>(), "x",
                                sol::property(&LuaBlock::rec_x, &LuaBlock::mettre_x), "y",
                                sol::property(&LuaBlock::rec_y, &LuaBlock::mettre_y), "w",
                                sol::property(&LuaBlock::rec_w, &LuaBlock::mettre_w), "h",
                                sol::property(&LuaBlock::rec_h, &LuaBlock::mettre_h), "type",
                                sol::property(&LuaBlock::rec_type, &LuaBlock::mettre_type));

    /* Blocks */
    game.new_usertype<LuaBlocks>("Blocks", sol::call_constructor, sol::constructors<LuaBlocks()>(),
                                 "add", &LuaBlocks::add, "size", &LuaBlocks::size, "get",
                                 &LuaBlocks::get, sol::meta_function::index, &LuaBlocks::operator[],
                                 "pairs", &LuaBlocks::pairs);

    /* EntityTopdown */
    game.new_usertype<LuaEntityTopdown>(
        "EntityTopdown", sol::call_constructor,
        sol::constructors<LuaEntityTopdown(float, float, float, float)>(), "x",
        sol::property(&LuaEntityTopdown::rec_x, &LuaEntityTopdown::mettre_x), "y",
        sol::property(&LuaEntityTopdown::rec_y, &LuaEntityTopdown::mettre_y), "w",
        sol::property(&LuaEntityTopdown::rec_w, &LuaEntityTopdown::mettre_w), "h",
        sol::property(&LuaEntityTopdown::rec_h, &LuaEntityTopdown::mettre_h));

    /* EntityPlatformer */
    game.new_usertype<LuaEntityPlatformer>(
        "EntityPlatformer", sol::call_constructor,
        sol::constructors<LuaEntityPlatformer(float, float, float, float, sol::optional<float>,
                                              sol::optional<float>)>(),
        "x", sol::property(&LuaEntityPlatformer::rec_x, &LuaEntityPlatformer::mettre_x), "y",
        sol::property(&LuaEntityPlatformer::rec_y, &LuaEntityPlatformer::mettre_y), "w",
        sol::property(&LuaEntityPlatformer::rec_w, &LuaEntityPlatformer::mettre_w), "h",
        sol::property(&LuaEntityPlatformer::rec_h, &LuaEntityPlatformer::mettre_h), "gravity",
        sol::property(&LuaEntityPlatformer::rec_gravite, &LuaEntityPlatformer::mettre_gravite),
        "jumpPower",
        sol::property(&LuaEntityPlatformer::rec_force_saut,
                      &LuaEntityPlatformer::mettre_force_saut),
        "requestJump",
        sol::property(&LuaEntityPlatformer::rec_demande_saut,
                      &LuaEntityPlatformer::mettre_demande_saut),
        "leftLock",
        sol::property(&LuaEntityPlatformer::rec_verrou_gauche,
                      &LuaEntityPlatformer::mettre_verrou_gauche),
        "rightLock",
        sol::property(&LuaEntityPlatformer::rec_verrou_droit,
                      &LuaEntityPlatformer::mettre_verrou_droit),
        "inSky",
        sol::property(&LuaEntityPlatformer::rec_en_air, &LuaEntityPlatformer::mettre_en_air),
        "speedY",
        sol::property(&LuaEntityPlatformer::rec_vitesseY, &LuaEntityPlatformer::mettre_vitesseY));

    /* Camera globale */
    game.set_function("createCamera", &creer_camera);
    game.set_function("updateCamera", &lua_update_camera);
    game.set_function("setCameraX", &lua_set_cam_x);
    game.set_function("getCameraX", &lua_get_cam_x);
    game.set_function("setCameraY", &lua_set_cam_y);
    game.set_function("getCameraY", &lua_get_cam_y);
    game.set_function("setCameraSmooth", &lua_set_cam_smooth);
    game.set_function("getCameraSmooth", &lua_get_cam_smooth);

    /* BlocksFromFile */
    game.new_usertype<LuaBlocksFromFile>(
        "BlocksFromFile", sol::call_constructor,
        sol::constructors<LuaBlocksFromFile(const std::string &, int, int, char, int)>(), "size",
        &LuaBlocksFromFile::size);

    /* hitbox functions */
    game.set_function("hitboxPlatformer",
                      sol::overload(&lua_hitbox_platformer, &lua_hitbox_platformer_table));
    game.set_function("hitboxTopdown",
                      sol::overload(&lua_hitbox_topdown, &lua_hitbox_topdown_table));
    game.set_function("animate", &lua_animation);
}