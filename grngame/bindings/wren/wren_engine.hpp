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

    WrenHandle *main_class = nullptr;

    WrenHandle *on_start = nullptr;
    WrenHandle *on_update = nullptr;
    WrenHandle *on_fixed_update = nullptr;
    WrenHandle *on_render = nullptr;
    WrenHandle *on_destroy = nullptr;

  public:
    WrenEngine();
    ~WrenEngine();

    bool Init(const char *main_script_name);

    bool CallOnStart() const;
    bool CallOnUpdate(float delta) const;
    bool CallOnFixedUpdate(float delta) const;
    bool CallOnRender() const;
    bool CallOnDestroy() const;

    WrenVM *GetVM() const
    {
        return vm;
    }

  private:
    bool LoadMainScript(const char *main_script_name);

    void ReleaseHandle(WrenHandle *&handle) const;

    static void WriteCallback(WrenVM *vm, const char *text);
    static void ErrorCallback(WrenVM *vm, WrenErrorType type, const char *module, int line, const char *message);

    static WrenForeignMethodFn BindForeignMethodCallback(WrenVM *vm, const char *module, const char *class_name,
                                                         bool is_static, const char *signature);
    static WrenForeignClassMethods BindForeignClassCallback(WrenVM *vm, const char *module, const char *class_name);

    static WrenLoadModuleResult LoadModuleCallback(WrenVM *vm, const char *module_name);
    static void LoadModuleCompleteCallback(WrenVM *vm, const char *module_name, WrenLoadModuleResult result);
};

// template for bind

template <typename T> T wren_get(WrenVM *vm, int slot);

template <> inline int wren_get<int>(WrenVM *vm, int slot)
{
    return (int)wrenGetSlotDouble(vm, slot);
}
template <> inline float wren_get<float>(WrenVM *vm, int slot)
{
    return (float)wrenGetSlotDouble(vm, slot);
}
template <> inline double wren_get<double>(WrenVM *vm, int slot)
{
    return wrenGetSlotDouble(vm, slot);
}
template <> inline bool wren_get<bool>(WrenVM *vm, int slot)
{
    return wrenGetSlotBool(vm, slot);
}
template <> inline const char *wren_get<const char *>(WrenVM *vm, int slot)
{
    return wrenGetSlotString(vm, slot);
}

template <typename T> void wren_set(WrenVM *vm, int slot, T val);

template <> inline void wren_set(WrenVM *vm, int slot, int v)
{
    wrenSetSlotDouble(vm, slot, (double)v);
}
template <> inline void wren_set(WrenVM *vm, int slot, float v)
{
    wrenSetSlotDouble(vm, slot, (double)v);
}
template <> inline void wren_set(WrenVM *vm, int slot, double v)
{
    wrenSetSlotDouble(vm, slot, v);
}
template <> inline void wren_set(WrenVM *vm, int slot, bool v)
{
    wrenSetSlotBool(vm, slot, v);
}
template <> inline void wren_set(WrenVM *vm, int slot, const char *v)
{
    wrenSetSlotString(vm, slot, v);
}
