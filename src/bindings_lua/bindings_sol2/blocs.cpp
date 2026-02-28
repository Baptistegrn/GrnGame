/*
 * Utilitaires Lua et wrapper C++ pour les modules de jeu.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "sol/property.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../module_jeu/blocs/bloc.h"
#include "../../module_jeu/carte/carte.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
/* wrapper pour le type Bloc avec gestion memoire */
struct LuaBlock {
    Bloc *ptr;

    LuaBlock(float x, float y, float w, float h, int type) { ptr = creer_block(x, y, w, h, type); }

    ~LuaBlock() {
        if (ptr) {
            liberer_block(ptr);
            ptr = nullptr;
        }
    }

    /* accesseurs */
    float get_x() const { return ptr->x; }
    void set_x(float v) { ptr->x = v; }

    float get_y() const { return ptr->y; }
    void set_y(float v) { ptr->y = v; }

    float get_w() const { return ptr->w; }
    void set_w(float v) { ptr->w = v; }

    float get_h() const { return ptr->h; }
    void set_h(float v) { ptr->h = v; }

    int get_type() const { return ptr->type; }
    void set_type(int t) { ptr->type = t; }
};

/* wrapper pour le tableau de blocs */
struct LuaBlocks {
    Blocs *ptr;

    LuaBlocks() { ptr = creer_blocks(); }

    ~LuaBlocks() {
        if (ptr) {
            liberer_blocks(ptr);
            ptr = nullptr;
        }
    }

    /* manipulation du tableau */
    void add(const LuaBlock &block) { ajouter_block(ptr, block.ptr); }
    int size() const { return taille_blocks(ptr); }
    Bloc *get(int index) const { return &ptr->tab[index]; }
    Bloc &operator[](int index) { return ptr->tab[index]; }

    /* iterateur pour boucle for...in en Lua */
    std::function<Bloc *()> pairs() {
        int index = -1;
        int max_size = taille_blocks(ptr);
        Blocs *blocks_ptr = ptr;

        return [blocks_ptr, index, max_size]() mutable -> Bloc * {
            index++;
            if (index < max_size) {
                return &blocks_ptr->tab[index];
            }
            return nullptr;
        };
    }
};

/* wrapper pour les blocs charges depuis un fichier */
struct LuaBlocksFromFile {
    Blocs *ptr;
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

/* convertit une table lua en structure Blocs C */
static Blocs *blocks_from_table(sol::table t) {
    Blocs *blocks = creer_blocks();
    for (auto &kv : t) {
        LuaBlock &b = kv.second.as<LuaBlock &>();
        ajouter_block(blocks, b.ptr);
    }
    return blocks;
}

/* enregistrement des bindings blocs */
void enregistrer_bindings_blocs(sol::table &blocks) {

    /* Bloc brut (renvoye par iterateur/get) */
    blocks.new_usertype<Bloc>("BlockRaw", sol::no_constructor, "x", &Bloc::x, "y", &Bloc::y, "w",
                              &Bloc::w, "h", &Bloc::h, "type", &Bloc::type);

    /* Constructeur de Bloc */
    blocks.new_usertype<LuaBlock>("Block", sol::call_constructor,
                                  sol::constructors<LuaBlock(float, float, float, float, int)>(),
                                  "x", sol::property(&LuaBlock::get_x, &LuaBlock::set_x), "y",
                                  sol::property(&LuaBlock::get_y, &LuaBlock::set_y), "w",
                                  sol::property(&LuaBlock::get_w, &LuaBlock::set_w), "h",
                                  sol::property(&LuaBlock::get_h, &LuaBlock::set_h), "type",
                                  sol::property(&LuaBlock::get_type, &LuaBlock::set_type));

    /* Tableau de Blocs */
    blocks.new_usertype<LuaBlocks>(
        "Blocks", sol::call_constructor, sol::constructors<LuaBlocks()>(), "add", &LuaBlocks::add,
        "size", &LuaBlocks::size, "get", &LuaBlocks::get, sol::meta_function::index,
        &LuaBlocks::operator[], "pairs", &LuaBlocks::pairs);
}