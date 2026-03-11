#include "renderer_module.hpp"

extern "C"
{
#include "grngame/renderer/particule.h"
#include "grngame/renderer/primitive.h"
#include "grngame/renderer/renderer.h"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
}

struct ParticleEmitterAnnotation : das::ManagedStructureAnnotation<ParticleEmitter, false>
{
    ParticleEmitterAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ParticleEmitter", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(lifetime)>("lifetime");
        addField<DAS_BIND_MANAGED_FIELD(explosiveness)>("explosiveness");
        addField<DAS_BIND_MANAGED_FIELD(one_shot)>("one_shot");
        addField<DAS_BIND_MANAGED_FIELD(local_coords)>("local_coords");
        addField<DAS_BIND_MANAGED_FIELD(direction)>("direction");
        addField<DAS_BIND_MANAGED_FIELD(spread)>("spread");
        addField<DAS_BIND_MANAGED_FIELD(initial_velocity)>("initial_velocity");
        addField<DAS_BIND_MANAGED_FIELD(gravity)>("gravity");
        addField<DAS_BIND_MANAGED_FIELD(damping)>("damping");
        addField<DAS_BIND_MANAGED_FIELD(scale_start)>("scale_start");
        addField<DAS_BIND_MANAGED_FIELD(scale_end)>("scale_end");
        addField<DAS_BIND_MANAGED_FIELD(scale_amount_curve)>("scale_amount_curve");
        addField<DAS_BIND_MANAGED_FIELD(amount)>("amount");
        addField<DAS_BIND_MANAGED_FIELD(rs)>("rs");
        addField<DAS_BIND_MANAGED_FIELD(gs)>("gs");
        addField<DAS_BIND_MANAGED_FIELD(bs)>("bs");
        addField<DAS_BIND_MANAGED_FIELD(as)>("as");
        addField<DAS_BIND_MANAGED_FIELD(re)>("re");
        addField<DAS_BIND_MANAGED_FIELD(ge)>("ge");
        addField<DAS_BIND_MANAGED_FIELD(be)>("be");
        addField<DAS_BIND_MANAGED_FIELD(ae)>("ae");
        addField<DAS_BIND_MANAGED_FIELD(emit_accumulator)>("emit_accumulator");
        addField<DAS_BIND_MANAGED_FIELD(emit_cycle_time)>("emit_cycle_time");
        addField<DAS_BIND_MANAGED_FIELD(one_shot_done)>("one_shot_done");
    }
};

struct SpriteAnnotation : das::ManagedStructureAnnotation<Sprite, false>
{
    SpriteAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("Sprite", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(w)>("w");
        addField<DAS_BIND_MANAGED_FIELD(h)>("h");
        addField<DAS_BIND_MANAGED_FIELD(name)>("name");
    }
};

static bool texture_draw(const char *name, float x, float y, int c = 1, bool f = false, int r = 0, int a = 255)
{
    return TextureDraw(name, x, y, (uint8)c, f, (int16)r, (uint8)a);
}
static bool texture_draw_i(const char *name, int x, int y, int c = 1, bool f = false, int r = 0, int a = 255)
{
    return TextureDraw(name, (float)x, (float)y, (uint8)c, f, (int16)r, (uint8)a);
}

static bool sprite_draw(const Sprite &s, int frame = 0, float x = 0, float y = 0, int c = 1, bool f = false, int r = 0,
                        int a = 255)
{
    return SpriteDraw(s, (uint16)frame, x, y, (uint8)c, f, (int16)r, (uint8)a);
}
static bool sprite_draw_i(const Sprite &s, int frame = 0, int x = 0, int y = 0, int c = 1, bool f = false, int r = 0,
                          int a = 255)
{
    return SpriteDraw(s, (uint16)frame, (float)x, (float)y, (uint8)c, f, (int16)r, (uint8)a);
}

static Sprite sprite_i(const char *name, int w, int h)
{
    return Sprite{(uint16)w, (uint16)h, nullptr, name};
}

static Sprite sprite_f(const char *name, float w, float h)
{
    return Sprite{(uint16)w, (uint16)h, nullptr, name};
}

static void pixel_draw(float x, float y, int r = 255, int g = 255, int b = 255, int a = 255)
{
    PixelDraw(x, y, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}
static void pixel_draw_i(int x, int y, int r = 255, int g = 255, int b = 255, int a = 255)
{
    PixelDraw((float)x, (float)y, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void line_draw(float x0, float y0, float x1, float y1, int r = 255, int g = 255, int b = 255, int a = 255)
{
    LineDraw(x0, y0, x1, y1, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}
static void line_draw_i(int x0, int y0, int x1, int y1, int r = 255, int g = 255, int b = 255, int a = 255)
{
    LineDraw((float)x0, (float)y0, (float)x1, (float)y1, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void rect_draw(float x, float y, float w, float h, int r = 255, int g = 255, int b = 255, int a = 255)
{
    RectDraw(x, y, w, h, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}
static void rect_draw_i(int x, int y, int w, int h, int r = 255, int g = 255, int b = 255, int a = 255)
{
    RectDraw((float)x, (float)y, (float)w, (float)h, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void rect_fill(float x, float y, float w, float h, int r = 255, int g = 255, int b = 255, int a = 255)
{
    RectDrawFill(x, y, w, h, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}
static void rect_fill_i(int x, int y, int w, int h, int r = 255, int g = 255, int b = 255, int a = 255)
{
    RectDrawFill((float)x, (float)y, (float)w, (float)h, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void circle_draw(float x, float y, float radius, int r = 255, int g = 255, int b = 255, int a = 255)
{
    CircleDraw(x, y, radius, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}
static void circle_draw_i(int x, int y, int radius, int r = 255, int g = 255, int b = 255, int a = 255)
{
    CircleDraw((float)x, (float)y, (float)radius, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void circle_fill(float x, float y, float radius, int r = 255, int g = 255, int b = 255, int a = 255)
{
    CircleDrawFill(x, y, radius, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}
static void circle_fill_i(int x, int y, int radius, int r = 255, int g = 255, int b = 255, int a = 255)
{
    CircleDrawFill((float)x, (float)y, (float)radius, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void cls(int r, int g, int b)
{
    SetRenderColor((uint8)r, (uint8)g, (uint8)b);
}

static ParticleEmitter particle_emitter_create()
{
    return ParticleEmitterCreate();
}
static void particle_emitter_destroy(ParticleEmitter &emit)
{
    ParticleEmitterDestroy(&emit);
}

static void emitter_update(ParticleEmitter &emit, float dt)
{
    UpdateEmitter(&emit, dt);
}
static void emitter_render(ParticleEmitter &emit)
{
    RenderEmitter(&emit);
}
RendererModule::RendererModule() : Module("grngame_renderer")
{
    auto module_library = das::ModuleLibrary(this);

    module_library.addBuiltInModule();
    module_library.addModule(this);

    addAnnotation(das::make_smart<ParticleEmitterAnnotation>(module_library));
    addAnnotation(das::make_smart<SpriteAnnotation>(module_library));

    das::addExtern<DAS_BIND_FUN(particle_emitter_create), das::SimNode_ExtFuncCallAndCopyOrMove>(
        *this, module_library, "particle_emitter_create", das::SideEffects::modifyExternal, "particle_emitter_create");
    das::addExtern<DAS_BIND_FUN(particle_emitter_destroy)>(*this, module_library, "particle_emitter_destroy",
                                                           das::SideEffects::modifyExternal,
                                                           "particle_emitter_destroy");

    das::addExtern<DAS_BIND_FUN(emitter_update)>(*this, module_library, "emitter_update",
                                                 das::SideEffects::modifyExternal, "emitter_update");
    das::addExtern<DAS_BIND_FUN(emitter_render)>(*this, module_library, "emitter_render",
                                                 das::SideEffects::modifyExternal, "emitter_render");

    das::addExtern<DAS_BIND_FUN(sprite_i), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, module_library, "sprite",
                                                                                  das::SideEffects::none, "sprite_i");
    das::addExtern<DAS_BIND_FUN(sprite_f), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, module_library, "sprite",
                                                                                  das::SideEffects::none, "sprite_f");

    das::addExtern<DAS_BIND_FUN(texture_draw)>(*this, module_library, "draw", das::SideEffects::modifyExternal,
                                               "texture_draw");
    das::addExtern<DAS_BIND_FUN(texture_draw_i)>(*this, module_library, "draw", das::SideEffects::modifyExternal,
                                                 "texture_draw_i");
    das::addExtern<DAS_BIND_FUN(sprite_draw)>(*this, module_library, "sprite_draw", das::SideEffects::modifyExternal,
                                              "sprite_draw");
    das::addExtern<DAS_BIND_FUN(sprite_draw_i)>(*this, module_library, "sprite_draw", das::SideEffects::modifyExternal,
                                                "sprite_draw_i");
    das::addExtern<DAS_BIND_FUN(pixel_draw)>(*this, module_library, "pixel_draw", das::SideEffects::modifyExternal,
                                             "pixel_draw");
    das::addExtern<DAS_BIND_FUN(pixel_draw_i)>(*this, module_library, "pixel_draw", das::SideEffects::modifyExternal,
                                               "pixel_draw_i");
    das::addExtern<DAS_BIND_FUN(line_draw)>(*this, module_library, "line_draw", das::SideEffects::modifyExternal,
                                            "line_draw");
    das::addExtern<DAS_BIND_FUN(line_draw_i)>(*this, module_library, "line_draw", das::SideEffects::modifyExternal,
                                              "line_draw_i");
    das::addExtern<DAS_BIND_FUN(rect_draw)>(*this, module_library, "rect_draw", das::SideEffects::modifyExternal,
                                            "rect_draw");
    das::addExtern<DAS_BIND_FUN(rect_draw_i)>(*this, module_library, "rect_draw", das::SideEffects::modifyExternal,
                                              "rect_draw_i");
    das::addExtern<DAS_BIND_FUN(rect_fill)>(*this, module_library, "rect_fill", das::SideEffects::modifyExternal,
                                            "rect_fill");
    das::addExtern<DAS_BIND_FUN(rect_fill_i)>(*this, module_library, "rect_fill", das::SideEffects::modifyExternal,
                                              "rect_fill_i");
    das::addExtern<DAS_BIND_FUN(circle_draw)>(*this, module_library, "circle_draw", das::SideEffects::modifyExternal,
                                              "circle_draw");
    das::addExtern<DAS_BIND_FUN(circle_draw_i)>(*this, module_library, "circle_draw", das::SideEffects::modifyExternal,
                                                "circle_draw_i");
    das::addExtern<DAS_BIND_FUN(circle_fill)>(*this, module_library, "circle_fill", das::SideEffects::modifyExternal,
                                              "circle_fill");
    das::addExtern<DAS_BIND_FUN(circle_fill_i)>(*this, module_library, "circle_fill", das::SideEffects::modifyExternal,
                                                "circle_fill_i");
    das::addExtern<DAS_BIND_FUN(cls)>(*this, module_library, "cls", das::SideEffects::modifyExternal, "cls");
}

REGISTER_MODULE(RendererModule);
