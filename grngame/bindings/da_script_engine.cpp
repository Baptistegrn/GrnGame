#include "da_script_engine.hpp"
#include "daScript/ast/dyn_modules.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/directories.h"
#include <cstdio>
#include <filesystem>

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

bool DaScriptEngine::CompileScript(const char *script_name)
{
    if (contexts.find(script_name) != contexts.end())
    {
        LOG_ERROR("Tried to compile script '%s' which was already compiled", script_name);
        return false;
    }

    auto script_path = std::filesystem::path(DirOfExecutable()) / "scripts" / (std::string(script_name) + ".das");
    auto program = das::compileDaScript(script_path, file_access, text_printer, module_group, policies);
    if (program->failed())
    {
        LOG_ERROR("Failed to compile daScript file '%s':\n", script_name);
        LogErrorsOfProgram(program);
        return false;
    }

    contexts[script_name] = das::make_smart<das::Context>(program->getContextStackSize());

    if (!program->simulate(*contexts[script_name], text_printer))
    {
        LOG_ERROR("Failed to simulate script '%s':", script_name);
        LogErrorsOfProgram(program);
        return false;
    }

    LOG_INFO("Sucesfully compiled script '%s'", script_name);

    return true;
}

bool DaScriptEngine::RunScript(const char *script_name, const char *entry)
{
    auto it = contexts.find(script_name);
    if (it == contexts.end())
    {
        LOG_ERROR("No such script '%s'\n", script_name);
        return false;
    }

    auto functions = it->second->findFunctions(entry);

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
        contexts[script_name]->evalWithCatch(functions.at(0), nullptr);
        return true;
    }
}

void DaScriptEngine::LogErrorsOfProgram(const das::ProgramPtr &program)
{
    for (const auto &err : program->errors)
    {
        LOG_ERROR("- %s\n", err.what.c_str()); // TODO better logging
    }
}
