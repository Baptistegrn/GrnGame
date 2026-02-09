/*
 * Utilitaires Lua et wrapper C++ pour les modules de jeu.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>
#include <vector>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* classe block */
struct LuaBlock {
    Block *ptr;

    LuaBlock(float x, float y, float w, float h, int type) { ptr = createBlock(x, y, w, h, type); }

    ~LuaBlock() {
        if (ptr) {
            freeBlock(ptr);
            ptr = nullptr;
        }
    }

    /* pour modifier/ recuperer les parametres */
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

/* classe blocks */
struct LuaBlocks {
    Blocks *ptr;

    LuaBlocks() { ptr = createBlocks(); }

    ~LuaBlocks() {
        if (ptr) {
            freeBlocks(ptr);
            ptr = nullptr;
        }
    }

    void add(const LuaBlock &block) { addBlock(ptr, block.ptr); }

    int size() const { return getBlocksSize(ptr); }

    /* boucle sur les blocs types */
    Block *get(int index) const { return &ptr->tab[index]; }

    /* Acces par indice [] */
    Block &operator[](int index) { return ptr->tab[index]; }

    /* Iterator pour for...in */
    std::function<Block *()> pairs() {
        int index = -1;
        int max_size = getBlocksSize(ptr);
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

/*classe entite topdown */
struct LuaEntityTopdown {
    EntityTopdown *ptr;

    LuaEntityTopdown(float x, float y, float w, float h) { ptr = createEntityTopdown(x, y, w, h); }

    ~LuaEntityTopdown() {
        if (ptr) {
            freeEntityTopdown(ptr);
            ptr = nullptr;
        }
    }

    /* pour modifier / recuperer les parametres */
    float rec_x() const { return ptr->x; }
    void mettre_x(float v) { ptr->x = v; }

    float rec_y() const { return ptr->y; }
    void mettre_y(float v) { ptr->y = v; }

    float rec_w() const { return ptr->width; }
    void mettre_w(float v) { ptr->width = v; }

    float rec_h() const { return ptr->height; }
    void mettre_h(float v) { ptr->height = v; }
};

/* classe entite platformer */
struct LuaEntityPlatformer {
    EntityPlatformer *ptr;

    LuaEntityPlatformer(float x, float y, float w, float h, sol::optional<float> force_saut,
                        sol::optional<float> gravite) {
        float f = force_saut.value_or(DEFAULT_FORCE_SAUT);
        float g = gravite.value_or(DEFAULT_GRAVITE);
        ptr = createEntityPlatformer(x, y, w, h, f, g);
    }

    ~LuaEntityPlatformer() {
        if (ptr) {
            freeEntityPlatformer(ptr);
            ptr = nullptr;
        }
    }

    /* pour modifier / recuperer les parametres */
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

    float rec_demande_saut() const { return ptr->requestJump; }
    void mettre_demande_saut(float v) { ptr->requestJump = v; }

    float rec_verrou_gauche() const { return ptr->leftLock; }
    void mettre_verrou_gauche(float v) { ptr->leftLock = v; }

    float rec_verrou_droit() const { return ptr->rightLock; }
    void mettre_verrou_droit(float v) { ptr->rightLock = v; }

    float rec_en_air() const { return ptr->inSky; }
    void mettre_en_air(float v) { ptr->inSky = v; }

    float rec_vitesseY() const { return ptr->speedY; }
    void mettre_vitesseY(float v) { ptr->speedY = v; }
};

/* classe camera */
struct LuaCamera {
    Camera *ptr;

    LuaCamera(float x, float y, float smooth, int w, int h) {
        ptr = createCamera(x, y, smooth, w, h);
    }

    ~LuaCamera() {
        if (ptr) {
            freeCamera(ptr);
            ptr = nullptr;
        }
    }

    /* pour recuerer/modifier les parametres */
    float rec_x() const { return ptr->x; }
    void mettre_x(float v) { ptr->x = v; }

    float rec_y() const { return ptr->y; }
    void mettre_y(float v) { ptr->y = v; }

    float rec_smooth() const { return ptr->smooth_factor; }
    void mettre_smooth(float v) { ptr->smooth_factor = v; }

    int rec_w() const { return ptr->width; }
    void mettre_w(int v) { ptr->width = v; }

    int rec_h() const { return ptr->height; }
    void mettre_h(int v) { ptr->height = v; }

    void update(float tx, float ty, float dt) { cameraUpdate(ptr, tx, ty, dt); }
};

/* classe pour les blocs charges depuis fichier */
struct LuaBlocksFromFile {
    Blocks *ptr;
    bool owns;

    LuaBlocksFromFile(const std::string &path, int pas_x, int pas_y, char sep, int exclude) {
        ptr = getBlocksFromFile(path.c_str(), pas_x, pas_y, sep, exclude);
        owns = true;
    }

    ~LuaBlocksFromFile() {
        if (ptr && owns) {
            freeBlocks(ptr);
            ptr = nullptr;
        }
    }

    int size() const { return getBlocksSize(ptr); }
};

/* convertir un tableau lua de block en une structure blocks */
static Blocks *blocks_depuis_tableau(sol::table t) {
    Blocks *blocks = createBlocks();

    for (auto &kv : t) {
        LuaBlock &b = kv.second.as<LuaBlock &>();
        addBlock(blocks, b.ptr);
    }
    return blocks;
}
/* hitblox platformer */
static EntityPlatformer *lua_hitbox_platformer(LuaEntityPlatformer &ent, LuaBlocks &blocks,
                                               float vmax, float correction,
                                               sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    /* type ignore */
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }

    return hitboxPlatformer(ent.ptr, blocks.ptr, vmax, correction, ig.data(), ig.size());
}
/* hitbox platformer avec table lua */
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
    EntityPlatformer *r = hitboxPlatformer(ent.ptr, blocks, v, c, ig.data(), ig.size());

    freeBlocks(blocks);
    return r;
}

/* hitbox topdown avec blocks */
static EntityTopdown *lua_hitbox_topdown(LuaEntityTopdown &ent, LuaBlocks &blocks,
                                         sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }
    return hitboxTopdown(ent.ptr, blocks.ptr, ig.data(), ig.size());
}

/* hitbox topdown avec table lua */
static EntityTopdown *lua_hitbox_topdown_table(LuaEntityTopdown &ent, sol::table t,
                                               sol::optional<sol::table> ignore) {
    std::vector<int> ig;
    if (ignore) {
        for (auto &kv : *ignore)
            ig.push_back(kv.second.as<int>());
    }

    Blocks *blocks = blocks_depuis_tableau(t);
    EntityTopdown *r = hitboxTopdown(ent.ptr, blocks, ig.data(), ig.size());
    freeBlocks(blocks);
    return r;
}

/* enregistrer les bindings module jeu */
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

    /* Camera */
    game.new_usertype<LuaCamera>(
        "Camera", sol::call_constructor,
        sol::constructors<LuaCamera(float, float, float, int, int)>(), "x",
        sol::property(&LuaCamera::rec_x, &LuaCamera::mettre_x), "y",
        sol::property(&LuaCamera::rec_y, &LuaCamera::mettre_y), "smoothFactor",
        sol::property(&LuaCamera::rec_smooth, &LuaCamera::mettre_smooth), "w",
        sol::property(&LuaCamera::rec_w, &LuaCamera::mettre_w), "h",
        sol::property(&LuaCamera::rec_h, &LuaCamera::mettre_h), "update", &LuaCamera::update);

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
}
