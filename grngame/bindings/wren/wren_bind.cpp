#include "grngame/bindings/wren/wren_bind.h"

#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/utils/attributes.h"


extern "C"
{
    WrenManager *WrenManagerNew()
    {
        return reinterpret_cast<WrenManager *>(new WrenEngine());
    }

    void WrenManagerDelete(WrenManager *manager)
    {
        delete reinterpret_cast<WrenEngine *>(manager);
    }

    bool WrenManagerInitialize(WrenManager *manager, const char *main_script_name)
    {
        return reinterpret_cast<WrenEngine *>(manager)->Init(main_script_name);
    }

    bool WrenManagerCallOnStart(WrenManager *manager)
    {
        return reinterpret_cast<WrenEngine *>(manager)->CallOnStart();
    }

    HOT bool WrenManagerCallOnUpdate(WrenManager *manager, float delta)
    {
        return reinterpret_cast<WrenEngine *>(manager)->CallOnUpdate(delta);
    }

    HOT bool WrenManagerCallOnFixedUpdate(WrenManager *manager, float delta)
    {
        return reinterpret_cast<WrenEngine *>(manager)->CallOnFixedUpdate(delta);
    }

    HOT bool WrenManagerCallOnRender(WrenManager *manager)
    {
        return reinterpret_cast<WrenEngine *>(manager)->CallOnRender();
    }

    bool WrenManagerCallOnDestroy(WrenManager *manager)
    {
        return reinterpret_cast<WrenEngine *>(manager)->CallOnDestroy();
    }

    void WrenManagerCollectGarbage(WrenManager *manager)
    {
        WrenVM *vm = reinterpret_cast<WrenEngine *>(manager)->GetVM();
        if (vm)
            wrenCollectGarbage(vm);
    }
}
