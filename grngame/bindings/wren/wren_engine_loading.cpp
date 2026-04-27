#include "grngame/bindings/wren/wren_engine.hpp"

#include <cstdlib>
#include <cstring>
#include <filesystem>

#include "grngame/assets/asset_manager.h"
#include "grngame/bindings/utils.hpp"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"

namespace
{
static const char *Basename(const char *path)
{
    if (!path)
        return "";

    const char *slash = std::strrchr(path, '/');
    const char *backslash = std::strrchr(path, '\\');
    const char *sep = slash;
    if (backslash && (!sep || backslash > sep))
        sep = backslash;
    return sep ? sep + 1 : path;
}

static const EmbeddedAsset *FindEmbeddedWrenAsset(const char *name)
{
    if (!g_app.info.embedded_assets || !name)
        return nullptr;

    const char *base = Basename(name);
    std::string name_with_ext = std::string(name) + ".wren";
    std::string base_with_ext = std::string(base) + ".wren";

    for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
    {
        const char *asset_name = g_app.info.embedded_assets[i].name;

        if (std::strcmp(asset_name, name) == 0)
            return &g_app.info.embedded_assets[i];
        if (std::strcmp(asset_name, name_with_ext.c_str()) == 0)
            return &g_app.info.embedded_assets[i];
        if (std::strcmp(asset_name, base) == 0)
            return &g_app.info.embedded_assets[i];
        if (std::strcmp(asset_name, base_with_ext.c_str()) == 0)
            return &g_app.info.embedded_assets[i];
    }

    return nullptr;
}

static bool LoadWrenSource(const char *name, std::string &out_source, const char *kind_for_log)
{
    const EmbeddedAsset *asset = FindEmbeddedWrenAsset(name);
    if (asset)
    {
        out_source.assign((const char *)asset->data, asset->size);
        LOG_DEBUG("[wren] Loaded embedded %s: '%s'", kind_for_log, name);
        return true;
    }

    const std::filesystem::path path = BuildModulePath(name, ".wren");
    if (!ReadTextFile(path, out_source))
    {
        LOG_ERROR("[wren] Failed to load %s '%s' from '%s'", kind_for_log, name, path.string().c_str());
        return false;
    }

    LOG_DEBUG("[wren] Loaded %s from disk: '%s'", kind_for_log, path.string().c_str());
    return true;
}
} // namespace

bool WrenEngine::LoadMainScript(const char *main_script_name)
{
    std::string main_source;
    if (!LoadWrenSource(main_script_name, main_source, "main script"))
        return false;

    const WrenInterpretResult result = wrenInterpret(vm, main_module.c_str(), main_source.c_str());
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Failed to interpret Wren script '%s.wren'", main_script_name);
        return false;
    }

    return true;
}

WrenLoadModuleResult WrenEngine::LoadModuleCallback(WrenVM *vm, const char *module_name)
{
    (void)vm;

    WrenLoadModuleResult result;
    result.onComplete = &WrenEngine::LoadModuleCompleteCallback;
    result.source = nullptr;
    result.userData = nullptr;

    std::string source;
    if (!LoadWrenSource(module_name, source, "module"))
        return result;

    char *owned_source = (char *)std::malloc(source.size() + 1);
    if (!owned_source)
    {
        LOG_ERROR("[wren] Failed to allocate source for module '%s'", module_name);
        return result;
    }

    std::memcpy(owned_source, source.data(), source.size());
    owned_source[source.size()] = '\0';
    result.source = owned_source;
    return result;
}

void WrenEngine::LoadModuleCompleteCallback(WrenVM *vm, const char *module_name, WrenLoadModuleResult result)
{
    (void)vm;
    (void)module_name;
    if (result.source)
        std::free((void *)result.source);
}
