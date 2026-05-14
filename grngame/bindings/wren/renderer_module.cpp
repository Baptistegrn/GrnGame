#include "grngame/bindings/wren/renderer_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
#include <cstdlib>
#include <cstring>

static void texture_draw_inner(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    double x = wren_get<double>(vm, 2);
    double y = wren_get<double>(vm, 3);
    uint8_t scale = (uint8_t)wren_get<double>(vm, 4);
    int16_t rot = (int16_t)wren_get<double>(vm, 5);
    uint8_t alpha = (uint8_t)wren_get<double>(vm, 6);
    wren_set<bool>(vm, 0, TextureDraw(name, (float)x, (float)y, scale, rot, alpha));
}

static void texture_width(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    ivec2s size = TextureGetSize(name);
    wren_set<double>(vm, 0, size.x);
}

static void texture_height(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    ivec2s size = TextureGetSize(name);
    wren_set<double>(vm, 0, size.y);
}

struct WrenSprite
{
    Sprite sprite;
    char *name_buf;
};

static void sprite_allocate(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(WrenSprite));
    const char *name = wren_get<const char *>(vm, 1);
    double w = wren_get<double>(vm, 2);
    double h = wren_get<double>(vm, 3);

    s->name_buf = strdup(name);
    s->sprite.w = (uint16_t)w;
    s->sprite.h = (uint16_t)h;
    s->sprite.tex = nullptr;
    s->sprite.name = s->name_buf;
}

static void sprite_finalize(void *data)
{
    WrenSprite *s = (WrenSprite *)data;
    if (s && s->name_buf)
    {
        free(s->name_buf);
        s->name_buf = nullptr;
    }
}

static void sprite_get_name(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    wren_set<const char *>(vm, 0, s->sprite.name);
}

static void sprite_set_name(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    const char *name = wren_get<const char *>(vm, 1);
    if (s->name_buf)
        free(s->name_buf);
    s->name_buf = strdup(name);
    s->sprite.name = s->name_buf;
    s->sprite.tex = nullptr; // reset cache
}

static void sprite_get_w(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    wren_set<double>(vm, 0, s->sprite.w);
}

static void sprite_set_w(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    s->sprite.w = (uint16_t)wren_get<double>(vm, 1);
}

static void sprite_get_h(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    wren_set<double>(vm, 0, s->sprite.h);
}

static void sprite_set_h(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    s->sprite.h = (uint16_t)wren_get<double>(vm, 1);
}

static void sprite_draw_inner(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    uint16_t frame = (uint16_t)wren_get<double>(vm, 1);
    double x = wren_get<double>(vm, 2);
    double y = wren_get<double>(vm, 3);
    uint8_t scale = (uint8_t)wren_get<double>(vm, 4);
    int16_t rot = (int16_t)wren_get<double>(vm, 5);
    uint8_t alpha = (uint8_t)wren_get<double>(vm, 6);
    wren_set<bool>(vm, 0, SpriteDraw(s->sprite, frame, (float)x, (float)y, scale, rot, alpha));
}

WrenForeignClassMethods BindForeignClassCallbackRenderer(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    if (strcmp(module, "std/wren/renderer/sprite") == 0 && strcmp(class_name, "Sprite") == 0)
    {
        return {sprite_allocate, sprite_finalize};
    }
    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackRenderer(WrenVM *vm, const char *module, const char *class_name,
                                                      bool is_static, const char *signature)
{
    (void)vm;
    if (!module || !class_name || !signature)
        return nullptr;

    if (strcmp(module, "std/wren/renderer/texture") == 0 && strcmp(class_name, "Texture") == 0 && is_static)
    {
        if (strcmp(signature, "draw_inner(_,_,_,_,_,_)") == 0)
            return texture_draw_inner;
        if (strcmp(signature, "width(_)") == 0)
            return texture_width;
        if (strcmp(signature, "height(_)") == 0)
            return texture_height;
    }

    if (strcmp(module, "std/wren/renderer/sprite") == 0 && strcmp(class_name, "Sprite") == 0 && !is_static)
    {
        if (strcmp(signature, "name") == 0)
            return sprite_get_name;
        if (strcmp(signature, "name=(_)") == 0)
            return sprite_set_name;
        if (strcmp(signature, "w") == 0)
            return sprite_get_w;
        if (strcmp(signature, "w=(_)") == 0)
            return sprite_set_w;
        if (strcmp(signature, "h") == 0)
            return sprite_get_h;
        if (strcmp(signature, "h=(_)") == 0)
            return sprite_set_h;

        if (strcmp(signature, "draw_inner(_,_,_,_,_,_)") == 0)
            return sprite_draw_inner;
    }

    return nullptr;
}