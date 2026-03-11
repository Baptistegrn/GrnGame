#include "renderer_module.hpp"

extern "C"
{
#include "grngame/renderer/primitive.h"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
}

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

static bool sprite_draw(const Sprite &s, int frame = 0, float x = 0, float y = 0, int c = 1, bool f = false, int r = 0,
                        int a = 255)
{
    return SpriteDraw(s, (uint16)frame, x, y, (uint8)c, f, (int16)r, (uint8)a);
}

static Sprite sprite(const char *name, int w, int h)
{
    return Sprite{(uint16)w, (uint16)h, nullptr, name};
}

static void pixel_draw(float x, float y, int r = 255, int g = 255, int b = 255, int a = 255)
{
    PixelDraw(x, y, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void line_draw(float x0, float y0, float x1, float y1, int r = 255, int g = 255, int b = 255, int a = 255)
{
    LineDraw(x0, y0, x1, y1, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void rect_draw(float x, float y, float w, float h, int r = 255, int g = 255, int b = 255, int a = 255)
{
    RectDraw(x, y, w, h, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void rect_fill(float x, float y, float w, float h, int r = 255, int g = 255, int b = 255, int a = 255)
{
    RectDrawFill(x, y, w, h, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void circle_draw(float x, float y, float radius, int r = 255, int g = 255, int b = 255, int a = 255)
{
    CircleDraw(x, y, radius, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

static void circle_fill(float x, float y, float radius, int r = 255, int g = 255, int b = 255, int a = 255)
{
    CircleDrawFill(x, y, radius, (uint8)r, (uint8)g, (uint8)b, (uint8)a);
}

RendererModule::RendererModule() : Module("grngame_renderer")
{
    auto module_library = das::ModuleLibrary(this);

    module_library.addBuiltInModule();
    module_library.addModule(this);

    addAnnotation(das::make_smart<SpriteAnnotation>(module_library));

    das::addExtern<DAS_BIND_FUN(sprite), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, module_library, "sprite",
                                                                                das::SideEffects::none, "sprite");

    das::addExtern<DAS_BIND_FUN(texture_draw)>(*this, module_library, "draw", das::SideEffects::modifyExternal,
                                               "texture_draw");
    das::addExtern<DAS_BIND_FUN(sprite_draw)>(*this, module_library, "sprite_draw", das::SideEffects::modifyExternal,
                                              "sprite_draw");
    das::addExtern<DAS_BIND_FUN(pixel_draw)>(*this, module_library, "pixel_draw", das::SideEffects::modifyExternal,
                                             "pixel_draw");
    das::addExtern<DAS_BIND_FUN(line_draw)>(*this, module_library, "line_draw", das::SideEffects::modifyExternal,
                                            "line_draw");
    das::addExtern<DAS_BIND_FUN(rect_draw)>(*this, module_library, "rect_draw", das::SideEffects::modifyExternal,
                                            "rect_draw");
    das::addExtern<DAS_BIND_FUN(rect_fill)>(*this, module_library, "rect_fill", das::SideEffects::modifyExternal,
                                            "rect_fill");
    das::addExtern<DAS_BIND_FUN(circle_draw)>(*this, module_library, "circle_draw", das::SideEffects::modifyExternal,
                                              "circle_draw");
    das::addExtern<DAS_BIND_FUN(circle_fill)>(*this, module_library, "circle_fill", das::SideEffects::modifyExternal,
                                              "circle_fill");
}

REGISTER_MODULE(RendererModule);
