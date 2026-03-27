#include "utils_module.hpp"

struct vec2sAnnotation : das::ManagedStructureAnnotation<vec2s, false>
{
    vec2sAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("vec2s", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(x)>("x");
        addField<DAS_BIND_MANAGED_FIELD(y)>("y");
    }
};

struct ivec2sAnnotation : das::ManagedStructureAnnotation<ivec2s, false>
{
    ivec2sAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ivec2s", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(x)>("x");
        addField<DAS_BIND_MANAGED_FIELD(y)>("y");
    }
};

UtilsModule::UtilsModule() : Module("grngame_utils")
{
    auto module_library = das::ModuleLibrary(this);
    module_library.addBuiltInModule();
    module_library.addModule(this);

    addAnnotation(das::make_smart<vec2sAnnotation>(module_library));
    addAnnotation(das::make_smart<ivec2sAnnotation>(module_library));
}

REGISTER_MODULE(UtilsModule)