#pragma once

#include <string>

extern "C"
{
#include <wren.h>
}

class WrenEngine
{
    WrenVM *vm = nullptr;
    std::string main_module;

    WrenHandle *on_start = nullptr;
    WrenHandle *on_update = nullptr;
    WrenHandle *on_fixed_update = nullptr;
    WrenHandle *on_render = nullptr;
    WrenHandle *on_destroy = nullptr;

    WrenHandle *call_zero_args = nullptr;
    WrenHandle *call_one_arg = nullptr;

  public:
    WrenEngine();
    ~WrenEngine();

    bool Init(const char *main_script_name);

    bool CallOnStart() const;
    bool CallOnUpdate(float delta) const;
    bool CallOnFixedUpdate(float delta) const;
    bool CallOnRender() const;
    bool CallOnDestroy() const;

  private:
    bool LoadMainScript(const char *main_script_name);
    WrenHandle *FindTopLevelFunction(const char *function_name) const;

    void ReleaseHandle(WrenHandle *&handle) const;

    static void WriteCallback(WrenVM *vm, const char *text);
    static void ErrorCallback(WrenVM *vm, WrenErrorType type, const char *module, int line, const char *message);
    static WrenLoadModuleResult LoadModuleCallback(WrenVM *vm, const char *module_name);
    static void LoadModuleCompleteCallback(WrenVM *vm, const char *module_name, WrenLoadModuleResult result);
};
