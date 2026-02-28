/*
 * Utilitaires Lua et wrapper C++ pour les modules de jeu.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "blocs.cpp"
#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../module_jeu/blocs/bloc.h"
#include "../../module_jeu/hitbox/hitbox.h"
#include "../../module_jeu/module_jeu.h"
#include "../../moteur/constante/constante.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* TODO: deplacer dans le bon fichier */
static std::tuple<int, float> lua_animate(int frame, float timer, int rangeStart, int rangeEnd,
                                          float speed, float dt, bool loop) {
    timer += dt;
    if (timer >= speed) {
        timer = 0;
        frame++;
        if (frame > rangeEnd) {
            frame = loop ? rangeStart : rangeEnd;
        }
    }
    return std::make_tuple(frame, timer);
}

/* wrapper pour une entite de plateforme avec gestion memoire */
struct LuaEntityPlatformer {
    EntitePlatforme *ptr;

    LuaEntityPlatformer(float x, float y, float w, float h, sol::optional<float> force_saut,
                        sol::optional<float> gravite, sol::optional<float> vmax_x,
                        sol::optional<float> vmax_chute, sol::optional<float> correction_mur,
                        sol::optional<float> vitesse_initiale, sol::optional<float> acceleration,
                        sol::optional<float> nb_sauts, sol::optional<bool> saut_sur_murs) {

        float f = force_saut.value_or(DEFAULT_FORCE_SAUT);
        float g = gravite.value_or(DEFAULT_GRAVITE);
        float vx = vmax_x.value_or(DEFAULT_VMAX_X);
        float vc = vmax_chute.value_or(DEFAULT_VMAX_CHUTE);
        float cm = correction_mur.value_or(DEFAULT_CORRECTION_MUR);
        float vi = vitesse_initiale.value_or(DEFAULT_VITESSE_INITIALE);
        float acc = acceleration.value_or(DEFAULT_ACCELERATION);
        int nb = nb_sauts.value_or(DEFAULT_NB_SAUTS_MAX);
        bool m = saut_sur_murs.value_or(DEFAULT_SAUT_SUR_MURS);

        ptr = creer_entite_platforme(x, y, w, h, f, g, vx, vc, cm, vi, acc, nb, m);
    }

    ~LuaEntityPlatformer() {
        if (ptr) {
            liberer_entite_platforme(ptr);
            ptr = nullptr;
        }
    }

    /* accesseurs ou si demande alors on attend le resultat de la hitbox */
    float get_x() const {
        attendre_calcul_hitbox();
        return ptr->x;
    }
    void set_x(float v) { ptr->x = v; }

    float get_y() const {
        attendre_calcul_hitbox();
        return ptr->y;
    }
    void set_y(float v) { ptr->y = v; }

    float get_w() const {
        attendre_calcul_hitbox();
        return ptr->width;
    }
    void set_w(float v) { ptr->width = v; }

    float get_h() const {
        attendre_calcul_hitbox();
        return ptr->height;
    }
    void set_h(float v) { ptr->height = v; }

    float get_powerJump() const {
        attendre_calcul_hitbox();
        return ptr->powerJump;
    }
    void set_powerJump(float v) { ptr->powerJump = v; }

    float get_gravity() const {
        attendre_calcul_hitbox();
        return ptr->gravity;
    }
    void set_gravity(float v) { ptr->gravity = v; }

    bool get_requestJump() const {
        attendre_calcul_hitbox();
        return ptr->requestJump;
    }
    void set_requestJump(bool v) { ptr->requestJump = v; }

    bool get_requestLeft() const {
        attendre_calcul_hitbox();
        return ptr->requestLeft;
    }
    void set_requestLeft(bool v) { ptr->requestLeft = v; }

    bool get_requestRight() const {
        attendre_calcul_hitbox();
        return ptr->requestRight;
    }
    void set_requestRight(bool v) { ptr->requestRight = v; }

    bool get_leftLock() const {
        attendre_calcul_hitbox();
        return ptr->leftLock;
    }
    void set_leftLock(bool v) { ptr->leftLock = v; }

    bool get_rightLock() const {
        attendre_calcul_hitbox();
        return ptr->rightLock;
    }
    void set_rightLock(bool v) { ptr->rightLock = v; }

    bool get_inSky() const {
        attendre_calcul_hitbox();
        return ptr->inSky;
    }
    void set_inSky(bool v) { ptr->inSky = v; }

    float get_speedY() const {
        attendre_calcul_hitbox();
        return ptr->speedY;
    }
    void set_speedY(float v) { ptr->speedY = v; }

    float get_speedX() const {
        attendre_calcul_hitbox();
        return ptr->speedX;
    }
    void set_speedX(float v) { ptr->speedX = v; }

    float get_acceleration() const {
        attendre_calcul_hitbox();
        return ptr->acceleration;
    }
    void set_acceleration(float v) { ptr->acceleration = v; }

    float get_initialSpeed() const {
        attendre_calcul_hitbox();
        return ptr->initialSpeed;
    }
    void set_initialSpeed(float v) { ptr->initialSpeed = v; }

    int get_numberOfJumps() const {
        attendre_calcul_hitbox();
        return ptr->numberOfJumps;
    }
    void set_numberOfJumps(int v) { ptr->numberOfJumps = v; }

    int get_numberOfJumpsPossible() const {
        attendre_calcul_hitbox();
        return ptr->numberOfJumpsPossible;
    }
    void set_numberOfJumpsPossible(int v) { ptr->numberOfJumpsPossible = v; }

    bool get_jumpOnWall() const {
        attendre_calcul_hitbox();
        return ptr->jumpOnWall;
    }
    void set_jumpOnWall(bool v) { ptr->jumpOnWall = v; }
};

/* wrapper pour le tableau d'entites */
struct LuaEntitiesPlatformer {
    EntitePlatformes *ptr;

    LuaEntitiesPlatformer() { ptr = creer_tableau_entite(); }

    ~LuaEntitiesPlatformer() {
        if (ptr) {
            liberer_entites_platforme(ptr);
            ptr = nullptr;
        }
    }

    /* manipulation du tableau */
    void add(const LuaEntityPlatformer &ent) { ajouter_entite_platforme(ptr, ent.ptr); }
    int size() const { return ptr->taille; }

    /* on ne met pas de securite ici car ca retourne juste un pointeur, c'est l'accesseur qui bloque
     */
    EntitePlatforme *get(int index) const { return &ptr->entites[index]; }
    EntitePlatforme &operator[](int index) { return ptr->entites[index]; }

    /* iterateur pour boucle for...in en Lua */
    std::function<EntitePlatforme *()> pairs() {
        int index = -1;
        int max_size = ptr->taille;
        EntitePlatformes *ents_ptr = ptr;

        return [ents_ptr, index, max_size]() mutable -> EntitePlatforme * {
            index++;
            if (index < max_size) {
                return &ents_ptr->entites[index];
            }
            return nullptr;
        };
    }
};

/* convertit une table lua en structure EntitePlatformes C */
static EntitePlatformes *entities_from_table(sol::table t) {
    EntitePlatformes *ents = creer_tableau_entite();
    for (auto &kv : t) {
        LuaEntityPlatformer &e = kv.second.as<LuaEntityPlatformer &>();
        ajouter_entite_platforme(ents, e.ptr);
    }
    return ents;
}

/* gestion de la hitbox avec les wrappers de tableaux (lance le calcul asynchrone) */
static void lua_hitbox_platformer(LuaEntitiesPlatformer &ents, LuaBlocks &blocks,
                                  sol::optional<Uint32> ignore_mask, sol::optional<float> delta) {
    float d = delta.value_or(dt());
    Uint32 i_mask = ignore_mask.value_or(0);
    lancer_calcul_hitbox(ents.ptr, blocks.ptr, i_mask, d);
}

/* gestion de la hitbox avec des tables Lua directes */
static void lua_hitbox_platformer_table(sol::table ents_table, sol::table blocks_table,
                                        sol::optional<Uint32> ignore_mask,
                                        sol::optional<float> delta) {
    float d = delta.value_or(dt());
    Uint32 i_mask = ignore_mask.value_or(0);

    EntitePlatformes *ents = entities_from_table(ents_table);
    Blocs *blocks = blocks_from_table(blocks_table);
    lancer_calcul_hitbox(ents, blocks, i_mask, d);

    /* pas de vrai multithread ici */
    attendre_calcul_hitbox();

    liberer_entites_platforme(ents);
    liberer_blocks(blocks);
}

/* enregistre les bindings de entite */
void enregistrer_bindings_entite(sol::table &entite) {
    /* Entite brute (renvoyee par iterateur/get) */
    entite.new_usertype<EntitePlatforme>(
        "EntityPlatformerRaw", sol::no_constructor, "x", &EntitePlatforme::x, "y",
        &EntitePlatforme::y, "speedY", &EntitePlatforme::speedY, "speedX", &EntitePlatforme::speedX,
        "w", &EntitePlatforme::width, "h", &EntitePlatforme::height, "gravity",
        &EntitePlatforme::gravity, "speedMaxX", &EntitePlatforme::speedMaxX, "powerJump",
        &EntitePlatforme::powerJump, "inSky", &EntitePlatforme::inSky, "requestJump",
        &EntitePlatforme::requestJump, "requestLeft", &EntitePlatforme::requestLeft, "requestRight",
        &EntitePlatforme::requestRight, "leftLock", &EntitePlatforme::leftLock, "rightLock",
        &EntitePlatforme::rightLock, "wallCorrection", &EntitePlatforme::wallCorrection,
        "speedMaxFall", &EntitePlatforme::speedMaxFall, "acceleration",
        &EntitePlatforme::acceleration, "initialSpeed", &EntitePlatforme::initialSpeed,
        "numberOfJumps", &EntitePlatforme::numberOfJumps, "numberOfJumpsPossible",
        &EntitePlatforme::numberOfJumpsPossible, "jumpOnWall", &EntitePlatforme::jumpOnWall);

    /* Constructeur d'entite */
    entite.new_usertype<LuaEntityPlatformer>(
        "EntityPlatformer", sol::call_constructor,
        sol::constructors<LuaEntityPlatformer(
            float, float, float, float, sol::optional<float>, sol::optional<float>,
            sol::optional<float>, sol::optional<float>, sol::optional<float>, sol::optional<float>,
            sol::optional<float>, sol::optional<float>, sol::optional<bool>)>(),
        "x", sol::property(&LuaEntityPlatformer::get_x, &LuaEntityPlatformer::set_x), "y",
        sol::property(&LuaEntityPlatformer::get_y, &LuaEntityPlatformer::set_y), "w",
        sol::property(&LuaEntityPlatformer::get_w, &LuaEntityPlatformer::set_w), "h",
        sol::property(&LuaEntityPlatformer::get_h, &LuaEntityPlatformer::set_h), "gravity",
        sol::property(&LuaEntityPlatformer::get_gravity, &LuaEntityPlatformer::set_gravity),
        "jumpPower",
        sol::property(&LuaEntityPlatformer::get_powerJump, &LuaEntityPlatformer::set_powerJump),
        "requestJump",
        sol::property(&LuaEntityPlatformer::get_requestJump, &LuaEntityPlatformer::set_requestJump),
        "requestLeft",
        sol::property(&LuaEntityPlatformer::get_requestLeft, &LuaEntityPlatformer::set_requestLeft),
        "requestRight",
        sol::property(&LuaEntityPlatformer::get_requestRight,
                      &LuaEntityPlatformer::set_requestRight),
        "leftLock",
        sol::property(&LuaEntityPlatformer::get_leftLock, &LuaEntityPlatformer::set_leftLock),
        "rightLock",
        sol::property(&LuaEntityPlatformer::get_rightLock, &LuaEntityPlatformer::set_rightLock),
        "inSky", sol::property(&LuaEntityPlatformer::get_inSky, &LuaEntityPlatformer::set_inSky),
        "speedY", sol::property(&LuaEntityPlatformer::get_speedY, &LuaEntityPlatformer::set_speedY),
        "speedX", sol::property(&LuaEntityPlatformer::get_speedX, &LuaEntityPlatformer::set_speedX),
        "acceleration",
        sol::property(&LuaEntityPlatformer::get_acceleration,
                      &LuaEntityPlatformer::set_acceleration),
        "initialSpeed",
        sol::property(&LuaEntityPlatformer::get_initialSpeed,
                      &LuaEntityPlatformer::set_initialSpeed),
        "numberOfJumps",
        sol::property(&LuaEntityPlatformer::get_numberOfJumps,
                      &LuaEntityPlatformer::set_numberOfJumps),
        "numberOfJumpsPossible",
        sol::property(&LuaEntityPlatformer::get_numberOfJumpsPossible,
                      &LuaEntityPlatformer::set_numberOfJumpsPossible),
        "jumpOnWall",
        sol::property(&LuaEntityPlatformer::get_jumpOnWall, &LuaEntityPlatformer::set_jumpOnWall));

    /* tableau entites */
    entite.new_usertype<LuaEntitiesPlatformer>(
        "EntitiesPlatformer", sol::call_constructor, sol::constructors<LuaEntitiesPlatformer()>(),
        "add", &LuaEntitiesPlatformer::add, "size", &LuaEntitiesPlatformer::size, "get",
        &LuaEntitiesPlatformer::get, sol::meta_function::index, &LuaEntitiesPlatformer::operator[],
        "pairs", &LuaEntitiesPlatformer::pairs);

    /* Fonctions hitbox surcharges */
    entite.set_function("hitboxPlatformer",
                        sol::overload(&lua_hitbox_platformer, &lua_hitbox_platformer_table));
    entite.set_function("animate", &lua_animate);
}