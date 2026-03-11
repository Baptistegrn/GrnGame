#include "da_script_engine.hpp"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/dyn_modules.h"
#include "daScript/daScriptModule.h"
#include "grngame/audio/speech.h"
#include "grngame/bindings/renderer_module.hpp"
#include "grngame/bindings/sound_module.hpp"
#include "grngame/dev/logging.h"
#include "grngame/platform/directories.h"
#include <filesystem>

DaScriptEngine::DaScriptEngine() : file_access(das::make_smart<das::FsFileAccess>())
{
    NEED_ALL_DEFAULT_MODULES;
    das::require_dynamic_modules(file_access, das::getDasRoot(), DirOfExecutable(), text_printer);

    policies.aot = true;
    policies.fail_on_no_aot = false;

    NEED_MODULE(RendererModule);
    NEED_MODULE(SoundModule);
    NEED_MODULE(Module_dasSQLITE);
}

DaScriptEngine::~DaScriptEngine()
{
    CallOnDestroy();
    das::Module::Shutdown();
}

bool DaScriptEngine::Init(const char *main_script_name)
{
    auto path_of_script =
        std::filesystem::path(DirOfExecutable()) / "scripts" / (std::string(main_script_name) + ".das");
    main_program = das::compileDaScript(path_of_script.string(), file_access, text_printer, module_group, policies);
    if (main_program->failed())
    {
        LOG_ERROR("Failed to compile script '%s':\n", main_script_name);
        LogErrorsOfProgram(main_program);
        return false;
    }

    main_context = std::make_unique<das::Context>(main_program->getContextStackSize());
    if (!main_program->simulate(*main_context, text_printer))
    {
        LOG_ERROR("Failed to simulate script '%s':\n", main_script_name);
        LogErrorsOfProgram(main_program);
        return false;
    }

    LOG_INFO("Sucesfully initialized dascript engine with script '%s'", main_script_name);

    on_start = main_context->findFunction("on_start");
    on_update = main_context->findFunction("on_update");
    on_fixed_update = main_context->findFunction("on_fixed_update");
    on_render = main_context->findFunction("on_render");
    on_destroy = main_context->findFunction("on_destroy");

    CallOnStart();

    return true;
}

bool DaScriptEngine::CallOnStart() const
{
    if (!on_start)
        return false;

    main_context->evalWithCatch(on_start);
    if (main_context->getException())
    {
        LOG_ERROR("Error in on_start: '%s'", main_context->getException());
        return false;
    }

    return true;
}

bool DaScriptEngine::CallOnUpdate(float delta) const
{
    if (!on_update)
        return false;

    vec4f args[1] = {das::cast<float>::from(delta)};
    main_context->evalWithCatch(on_update, args);
    if (main_context->getException())
    {
        LOG_ERROR("Error in on_update: '%s'", main_context->getException());
        return false;
    }

    return true;
}

bool DaScriptEngine::CallOnFixedUpdate(float delta) const
{
    if (!on_fixed_update)
        return false;

    vec4f args[1] = {das::cast<float>::from(delta)};
    main_context->evalWithCatch(on_fixed_update, args);
    if (main_context->getException())
    {
        LOG_ERROR("Error in on_fixed_update: '%s'", main_context->getException());
        return false;
    }

    return true;
}

bool DaScriptEngine::CallOnRender() const
{
    if (!on_render)
        return false;

    main_context->evalWithCatch(on_render);
    if (main_context->getException())
    {
        LOG_ERROR("Error in on_render: '%s'", main_context->getException());
        return false;
    }

    return true;
}

bool DaScriptEngine::CallOnDestroy() const
{
    if (!on_destroy)
        return false;

    main_context->evalWithCatch(on_destroy);
    if (main_context->getException())
    {
        LOG_ERROR("Error in on_destroy: '%s'", main_context->getException());
        return false;
    }

    return true;
}

void DaScriptEngine::LogErrorsOfProgram(const das::ProgramPtr &program)
{
    for (const auto &err : program->errors)
    {
        LOG_ERROR("- %s\n", err.what.c_str()); // TODO better logging
    }
}
