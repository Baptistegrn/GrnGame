#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/math/types.h"
#include "grngame/renderer/particule.h"
#include "grngame/renderer/primitive.h"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/string_compat.h"
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
    uint8 a = (uint8)wrenGetSlotDouble(vm, 6);
    wrenSetSlotBool(vm, 0, TextureDraw(name, (float)x, (float)y, scale, rot, a));
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
    uint8 a = (uint8)wrenGetSlotDouble(vm, 6);
    wrenSetSlotBool(vm, 0, SpriteDraw(s->sprite, frame, (float)x, (float)y, scale, rot, a));
}

HOT static void emitter_allocate(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(ParticleEmitter));
    *e = ParticleEmitterCreate(0);
}

COLD static void emitter_finalize(void *data)
{
    ParticleEmitter *e = (ParticleEmitter *)data;
    if (e)
        ParticleEmitterDestroy(e);
}

static void emitter_spawn(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    // SpawnParticle(e);
}

static void emitter_update(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    double dt = wrenGetSlotDouble(vm, 1);
    UpdateEmitter(e, (float)dt);
}

static void emitter_render(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    RenderEmitter(e);
}

static void emitter_get_pos_x(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->position.x);
}

static void emitter_set_pos_x(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->position.x = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_pos_y(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->position.y);
}

static void emitter_set_pos_y(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->position.y = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_capacity(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->capacity);
}

static void emitter_get_alive_count(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->alive_count);
}

static void emitter_get_lifetime(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->lifetime);
}

static void emitter_set_lifetime(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->lifetime = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_lifetime_variation(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->lifetime_variation);
}

static void emitter_set_lifetime_variation(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->lifetime_variation = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_explosiveness(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->explosiveness);
}

static void emitter_set_explosiveness(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->explosiveness = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_one_shot(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->one_shot);
}

static void emitter_set_one_shot(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->one_shot = wrenGetSlotBool(vm, 1);
}

static void emitter_get_one_shot_done(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->one_shot_done);
}

static void emitter_set_one_shot_done(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->one_shot_done = wrenGetSlotBool(vm, 1);
}

static void emitter_get_local_coords(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->local_coords);
}

static void emitter_set_local_coords(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->local_coords = wrenGetSlotBool(vm, 1);
}

static void emitter_get_direction(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->direction);
}

static void emitter_set_direction(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->direction = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_spread(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->spread);
}

static void emitter_set_spread(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->spread = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_initial_velocity(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->initial_velocity);
}

static void emitter_set_initial_velocity(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->initial_velocity = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_velocity_variation(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->velocity_variation);
}

static void emitter_set_velocity_variation(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->velocity_variation = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_gravity(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->gravity);
}

static void emitter_set_gravity(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->gravity = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_damping(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->damping);
}

static void emitter_set_damping(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->damping = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_turbulence(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->turbulence);
}

static void emitter_set_turbulence(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->turbulence = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_turbulence_freq(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->turbulence_freq);
}

static void emitter_set_turbulence_freq(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->turbulence_freq = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_scale_start(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->scale_start);
}

static void emitter_set_scale_start(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->scale_start = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_scale_end(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->scale_end);
}

static void emitter_set_scale_end(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->scale_end = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_scale_amount_curve(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->scale_amount_curve);
}

static void emitter_set_scale_amount_curve(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->scale_amount_curve = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_color_start(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->color_start);
}

static void emitter_set_color_start(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->color_start = (int32)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_alpha_start(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->alpha_start);
}

static void emitter_set_alpha_start(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->alpha_start = (uint8)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_color_end(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->color_end);
}

static void emitter_set_color_end(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->color_end = (int32)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_alpha_end(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->alpha_end);
}

static void emitter_set_alpha_end(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->alpha_end = (uint8)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_spawn_radius(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->spawn_radius);
}

static void emitter_set_spawn_radius(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->spawn_radius = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_spawn_rect_w(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->spawn_rect_w);
}

static void emitter_set_spawn_rect_w(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->spawn_rect_w = (float)wrenGetSlotDouble(vm, 1);
}

static void emitter_get_spawn_rect_h(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)e->spawn_rect_h);
}

static void emitter_set_spawn_rect_h(WrenVM *vm)
{
    ParticleEmitter *e = (ParticleEmitter *)wrenGetSlotForeign(vm, 0);
    e->spawn_rect_h = (float)wrenGetSlotDouble(vm, 1);
}

static void primitive_pixel(WrenVM *vm)
{
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    int c_idx = (int)wrenGetSlotDouble(vm, 3);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 4);
    PixelDraw((float)x, (float)y, c_idx, a);
}

static void primitive_line(WrenVM *vm)
{
    double x0 = wrenGetSlotDouble(vm, 1);
    double y0 = wrenGetSlotDouble(vm, 2);
    double x1 = wrenGetSlotDouble(vm, 3);
    double y1 = wrenGetSlotDouble(vm, 4);
    int c_idx = (int)wrenGetSlotDouble(vm, 5);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 6);
    LineDraw((float)x0, (float)y0, (float)x1, (float)y1, c_idx, a);
}

static void primitive_rect(WrenVM *vm)
{
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    double w = wrenGetSlotDouble(vm, 3);
    double h = wrenGetSlotDouble(vm, 4);
    int c_idx = (int)wrenGetSlotDouble(vm, 5);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 6);
    RectDraw((float)x, (float)y, (float)w, (float)h, c_idx, a);
}

static void primitive_rect_fill(WrenVM *vm)
{
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    double w = wrenGetSlotDouble(vm, 3);
    double h = wrenGetSlotDouble(vm, 4);
    int c_idx = (int)wrenGetSlotDouble(vm, 5);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 6);
    RectDrawFill((float)x, (float)y, (float)w, (float)h, c_idx, a);
}

static void primitive_circle(WrenVM *vm)
{
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    double radius = wrenGetSlotDouble(vm, 3);
    int c_idx = (int)wrenGetSlotDouble(vm, 4);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 5);
    CircleDraw((float)x, (float)y, (float)radius, c_idx, a);
}

static void primitive_circle_fill(WrenVM *vm)
{
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    double radius = wrenGetSlotDouble(vm, 3);
    int c_idx = (int)wrenGetSlotDouble(vm, 4);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 5);
    CircleDrawFill((float)x, (float)y, (float)radius, c_idx, a);
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

    const char *emit_module = "std/wren/renderer/particule";
    const char *emit_cls = "ParticleEmitter";

    RegisterClass(emit_module, emit_cls, emitter_allocate, emitter_finalize);

    // RegisterMethod(emit_module, emit_cls, false, "spawn()", emitter_spawn);

    RegisterMethod(emit_module, emit_cls, false, "update(_)", emitter_update);
    RegisterMethod(emit_module, emit_cls, false, "render()", emitter_render);

    RegisterMethod(emit_module, emit_cls, false, "pos_x", emitter_get_pos_x);
    RegisterMethod(emit_module, emit_cls, false, "pos_x=(_)", emitter_set_pos_x);
    RegisterMethod(emit_module, emit_cls, false, "pos_y", emitter_get_pos_y);
    RegisterMethod(emit_module, emit_cls, false, "pos_y=(_)", emitter_set_pos_y);

    RegisterMethod(emit_module, emit_cls, false, "capacity", emitter_get_capacity);
    RegisterMethod(emit_module, emit_cls, false, "alive_count", emitter_get_alive_count);

    RegisterMethod(emit_module, emit_cls, false, "lifetime", emitter_get_lifetime);
    RegisterMethod(emit_module, emit_cls, false, "lifetime=(_)", emitter_set_lifetime);
    RegisterMethod(emit_module, emit_cls, false, "lifetime_variation", emitter_get_lifetime_variation);
    RegisterMethod(emit_module, emit_cls, false, "lifetime_variation=(_)", emitter_set_lifetime_variation);

    RegisterMethod(emit_module, emit_cls, false, "explosiveness", emitter_get_explosiveness);
    RegisterMethod(emit_module, emit_cls, false, "explosiveness=(_)", emitter_set_explosiveness);

    RegisterMethod(emit_module, emit_cls, false, "one_shot", emitter_get_one_shot);
    RegisterMethod(emit_module, emit_cls, false, "one_shot=(_)", emitter_set_one_shot);
    RegisterMethod(emit_module, emit_cls, false, "one_shot_done", emitter_get_one_shot_done);
    RegisterMethod(emit_module, emit_cls, false, "one_shot_done=(_)", emitter_set_one_shot_done);

    RegisterMethod(emit_module, emit_cls, false, "local_coords", emitter_get_local_coords);
    RegisterMethod(emit_module, emit_cls, false, "local_coords=(_)", emitter_set_local_coords);

    RegisterMethod(emit_module, emit_cls, false, "direction", emitter_get_direction);
    RegisterMethod(emit_module, emit_cls, false, "direction=(_)", emitter_set_direction);

    RegisterMethod(emit_module, emit_cls, false, "spread", emitter_get_spread);
    RegisterMethod(emit_module, emit_cls, false, "spread=(_)", emitter_set_spread);

    RegisterMethod(emit_module, emit_cls, false, "initial_velocity", emitter_get_initial_velocity);
    RegisterMethod(emit_module, emit_cls, false, "initial_velocity=(_)", emitter_set_initial_velocity);
    RegisterMethod(emit_module, emit_cls, false, "velocity_variation", emitter_get_velocity_variation);
    RegisterMethod(emit_module, emit_cls, false, "velocity_variation=(_)", emitter_set_velocity_variation);

    RegisterMethod(emit_module, emit_cls, false, "gravity", emitter_get_gravity);
    RegisterMethod(emit_module, emit_cls, false, "gravity=(_)", emitter_set_gravity);

    RegisterMethod(emit_module, emit_cls, false, "damping", emitter_get_damping);
    RegisterMethod(emit_module, emit_cls, false, "damping=(_)", emitter_set_damping);
    RegisterMethod(emit_module, emit_cls, false, "turbulence", emitter_get_turbulence);
    RegisterMethod(emit_module, emit_cls, false, "turbulence=(_)", emitter_set_turbulence);
    RegisterMethod(emit_module, emit_cls, false, "turbulence_freq", emitter_get_turbulence_freq);
    RegisterMethod(emit_module, emit_cls, false, "turbulence_freq=(_)", emitter_set_turbulence_freq);

    RegisterMethod(emit_module, emit_cls, false, "scale_start", emitter_get_scale_start);
    RegisterMethod(emit_module, emit_cls, false, "scale_start=(_)", emitter_set_scale_start);
    RegisterMethod(emit_module, emit_cls, false, "scale_end", emitter_get_scale_end);
    RegisterMethod(emit_module, emit_cls, false, "scale_end=(_)", emitter_set_scale_end);
    RegisterMethod(emit_module, emit_cls, false, "scale_amount_curve", emitter_get_scale_amount_curve);
    RegisterMethod(emit_module, emit_cls, false, "scale_amount_curve=(_)", emitter_set_scale_amount_curve);

    RegisterMethod(emit_module, emit_cls, false, "color_start", emitter_get_color_start);
    RegisterMethod(emit_module, emit_cls, false, "color_start=(_)", emitter_set_color_start);
    RegisterMethod(emit_module, emit_cls, false, "alpha_start", emitter_get_alpha_start);
    RegisterMethod(emit_module, emit_cls, false, "alpha_start=(_)", emitter_set_alpha_start);

    RegisterMethod(emit_module, emit_cls, false, "color_end", emitter_get_color_end);
    RegisterMethod(emit_module, emit_cls, false, "color_end=(_)", emitter_set_color_end);
    RegisterMethod(emit_module, emit_cls, false, "alpha_end", emitter_get_alpha_end);
    RegisterMethod(emit_module, emit_cls, false, "alpha_end=(_)", emitter_set_alpha_end);

    RegisterMethod(emit_module, emit_cls, false, "spawn_radius", emitter_get_spawn_radius);
    RegisterMethod(emit_module, emit_cls, false, "spawn_radius=(_)", emitter_set_spawn_radius);
    RegisterMethod(emit_module, emit_cls, false, "spawn_rect_w", emitter_get_spawn_rect_w);
    RegisterMethod(emit_module, emit_cls, false, "spawn_rect_w=(_)", emitter_set_spawn_rect_w);
    RegisterMethod(emit_module, emit_cls, false, "spawn_rect_h", emitter_get_spawn_rect_h);
    RegisterMethod(emit_module, emit_cls, false, "spawn_rect_h=(_)", emitter_set_spawn_rect_h);

    const char *prim_module = "std/wren/renderer/primitive";
    const char *prim_cls = "Primitive";

    RegisterMethod(prim_module, prim_cls, true, "pixel(_,_,_,_)", primitive_pixel);
    RegisterMethod(prim_module, prim_cls, true, "line(_,_,_,_,_,_)", primitive_line);
    RegisterMethod(prim_module, prim_cls, true, "rect(_,_,_,_,_,_)", primitive_rect);
    RegisterMethod(prim_module, prim_cls, true, "rect_fill(_,_,_,_,_,_)", primitive_rect_fill);
    RegisterMethod(prim_module, prim_cls, true, "circle(_,_,_,_,_)", primitive_circle);
    RegisterMethod(prim_module, prim_cls, true, "circle_fill(_,_,_,_,_)", primitive_circle_fill);
}