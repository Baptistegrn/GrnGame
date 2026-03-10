#pragma once
#define DAS_SMART_PTR_TRACKER 0
#define DAS_SMART_PTR_MAGIC 0
#include <daScript/daScript.h>


class DaScriptEngine
{
    das::smart_ptr<das::FileAccess> file_access;
    das::TextPrinter text_printer;
    das::ModuleGroup module_group;
    das::CodeOfPolicies policies;
    das::ProgramPtr main_program;
    das::Context* main_context;
public:
    DaScriptEngine();
    ~DaScriptEngine();

    bool CompileScript(const char* script_name);
    bool RunScript(const char* script_name, const char* entry = "main");

private:
    static void LogErrorsOfProgram(const das::ProgramPtr& program);
};
