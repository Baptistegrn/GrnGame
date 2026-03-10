#pragma once
#define DAS_SMART_PTR_TRACKER 0
#define DAS_SMART_PTR_MAGIC 0
#include "daScript/ast/ast.h"
#include "daScript/simulate/simulate.h"
#include <unordered_map>
#include <daScript/daScript.h>
#include <optional>

class DaScriptEngine
{
    das::smart_ptr<das::FileAccess> file_access;
    das::TextPrinter text_printer;
    das::ModuleGroup module_group;
    das::CodeOfPolicies policies;
    das::ProgramPtr main_program;
    std::unique_ptr<das::Context> main_context;

    das::SimFunction* on_update = nullptr;
    das::SimFunction* on_start = nullptr;
    das::SimFunction* on_fixed_update = nullptr;
    das::SimFunction* on_render = nullptr;
    das::SimFunction* on_destroy = nullptr;
public:
    DaScriptEngine();
    ~DaScriptEngine();

    bool Init(const char* main_script_name);

    bool CallOnStart() const;
    bool CallOnUpdate(float delta) const;
    bool CallOnFixedUpdate(float delta) const;
    bool CallOnRender() const;
    bool CallOnDestroy() const;
private:
    static void LogErrorsOfProgram(const das::ProgramPtr& program);
};
