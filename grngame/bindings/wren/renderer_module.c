#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <stdlib.h>
#include <string.h>

static void texture_draw_inner(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    double x = wrenGetSlotDouble(vm, 2);
    double y = wrenGetSlotDouble(vm, 3);
    uint8_t scale = (uint8_t)wrenGetSlotDouble(vm, 4);
    int16_t rot = (int16_t)wrenGetSlotDouble(vm, 5);
    uint8_t alpha = (uint8_t)wrenGetSlotDouble(vm, 6);
    wrenSetSlotBool(vm, 0, TextureDraw(name, (float)x, (float)y, scale, rot, alpha));
}

static void texture_width(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    ivec2s size = TextureGetSize(name);
    wrenSetSlotDouble(vm, 0, (double)size.x);
}

static void texture_height(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    ivec2s size = TextureGetSize(name);
    wrenSetSlotDouble(vm, 0, (double)size.y);
}

typedef struct
{
    Sprite sprite;
    char *name_buf;
} WrenSprite;

HOT static void sprite_allocate(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(WrenSprite));
    const char *name = wrenGetSlotString(vm, 1);
    double w = wrenGetSlotDouble(vm, 2);
    double h = wrenGetSlotDouble(vm, 3);

    s->name_buf = strdup(name);
    s->sprite.w = (uint16_t)w;
    s->sprite.h = (uint16_t)h;
    s->sprite.tex = NULL;
    s->sprite.name = s->name_buf;
}

COLD static void sprite_finalize(void *data)
{
    WrenSprite *s = (WrenSprite *)data;
    if (s && s->name_buf)
    {
        free(s->name_buf);
        s->name_buf = NULL;
    }
}

static void sprite_get_name(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotString(vm, 0, s->sprite.name);
}

static void sprite_set_name(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    const char *name = wrenGetSlotString(vm, 1);
    if (s->name_buf)
        free(s->name_buf);
    s->name_buf = strdup(name);
    s->sprite.name = s->name_buf;
    s->sprite.tex = NULL;
}

static void sprite_get_w(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)s->sprite.w);
}

static void sprite_set_w(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    s->sprite.w = (uint16_t)wrenGetSlotDouble(vm, 1);
}

static void sprite_get_h(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)s->sprite.h);
}

static void sprite_set_h(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    s->sprite.h = (uint16_t)wrenGetSlotDouble(vm, 1);
}

static void sprite_draw_inner(WrenVM *vm)
{
    WrenSprite *s = (WrenSprite *)wrenGetSlotForeign(vm, 0);
    uint16_t frame = (uint16_t)wrenGetSlotDouble(vm, 1);
    double x = wrenGetSlotDouble(vm, 2);
    double y = wrenGetSlotDouble(vm, 3);
    uint8_t scale = (uint8_t)wrenGetSlotDouble(vm, 4);
    int16_t rot = (int16_t)wrenGetSlotDouble(vm, 5);
    uint8_t alpha = (uint8_t)wrenGetSlotDouble(vm, 6);
    wrenSetSlotBool(vm, 0, SpriteDraw(s->sprite, frame, (float)x, (float)y, scale, rot, alpha));
}

void RegisterRendererModule()
{
    const char *tex_module = "std/wren/renderer/texture";
    const char *tex_cls = "Texture";

    RegisterMethod(tex_module, tex_cls, true, "draw_inner(_,_,_,_,_,_)", texture_draw_inner);
    RegisterMethod(tex_module, tex_cls, true, "width(_)", texture_width);
    RegisterMethod(tex_module, tex_cls, true, "height(_)", texture_height);

    const char *spr_module = "std/wren/renderer/sprite";
    const char *spr_cls = "Sprite";

    RegisterClass(spr_module, spr_cls, sprite_allocate, sprite_finalize);

    RegisterMethod(spr_module, spr_cls, false, "name", sprite_get_name);
    RegisterMethod(spr_module, spr_cls, false, "name=(_)", sprite_set_name);
    RegisterMethod(spr_module, spr_cls, false, "w", sprite_get_w);
    RegisterMethod(spr_module, spr_cls, false, "w=(_)", sprite_set_w);
    RegisterMethod(spr_module, spr_cls, false, "h", sprite_get_h);
    RegisterMethod(spr_module, spr_cls, false, "h=(_)", sprite_set_h);
    RegisterMethod(spr_module, spr_cls, false, "draw_inner(_,_,_,_,_,_)", sprite_draw_inner);
}