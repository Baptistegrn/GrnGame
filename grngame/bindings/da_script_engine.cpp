#include "da_script_engine.hpp"
#include "daScript/ast/dyn_modules.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/directories.h"


DaScriptEngine::DaScriptEngine() : file_access(das::make_smart<das::FsFileAccess>())
{
    NEED_ALL_DEFAULT_MODULES;
    das::require_dynamic_modules(file_access, das::getDasRoot(), DirOfExecutable(), text_printer);

    policies.aot = true;
    policies.fail_on_no_aot = false;
}

DaScriptEngine::~DaScriptEngine()
{
    das::Module::Shutdown();
}

bool DaScriptEngine::CompileScript(const char* script_name)
{
    main_program = das::compileDaScript(script_name, file_access, text_printer, module_group, policies);
    if (main_program->failed())
    {
        LOG_ERROR("Failed to compile daScript file '%s':\n", script_name);
        LogErrorsOfProgram(main_program);
        return false;
    }

    main_context = new das::Context(main_program->getContextStackSize());

    if (!main_program->simulate(*main_context, text_printer))
    {
        LOG_ERROR("Failed to simulate script '%s':\n", script_name);
        LogErrorsOfProgram(main_program);
        return false;
    }

    return true;
}

bool DaScriptEngine::RunScript(const char* script_name, const char* entry)
{
    auto functions = main_context->findFunctions(entry);
    if (functions.empty())
    {
        LOG_ERROR("Failed to find function '%s' in script '%s'", entry, script_name);
        return false;
    }
    else if (functions.size() > 1)
    {
        LOG_ERROR("More than 1 functions named '%s' in script '%s'", entry, script_name);
        return false;
    }
    else
    {
        main_context->evalWithCatch(functions.at(0), nullptr);
        return true;
    }
}

void DaScriptEngine::LogErrorsOfProgram(const das::ProgramPtr& program)
{
    for (const auto& err : program->errors)
    {
        LOG_ERROR("- %s\n", err.what.c_str()); // TODO better logging
    }
}
