#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include "wren.h"

#include <stdbool.h>

BEGIN_DECLARATIONS

typedef void (*ModuleRegisterFn)(void);

#define MAX_MODULES 128
extern ModuleRegisterFn g_moduleRegistry[MAX_MODULES];
extern int g_moduleCount;

static inline void RegisterModuleEntry(ModuleRegisterFn fn)
{
    g_moduleRegistry[g_moduleCount++] = fn;
}

#define WREN_MODULE(fn)

typedef enum
{
    CB_ARG_NUM,
    CB_ARG_BOOL,
    CB_ARG_STRING,
} CallbackArgType;

typedef struct
{
    CallbackArgType type;
    union {
        double num;
        bool boolean;
        const char *string;
    } as;
} CallbackArg;

typedef struct
{
    bool is_registered;
    WrenHandle *handle;
    uint8 arity;
} WrenCallback;

typedef struct
{
    WrenCallback callbacks[4];
} WrenCallbackRegistry;

typedef struct
{
    WrenConfiguration config;
    WrenVM *vm;
    WrenHandle *main_class;

    WrenHandle *on_start;
    WrenHandle *on_update;
    WrenHandle *on_fixed_update;
    WrenHandle *on_render;
    WrenHandle *on_destroy;

    WrenHandle *call[17];

    WrenCallbackRegistry registry;

} WrenManager;

bool ReloadWrenScript(const char *filename);

bool WrenInit(void);

void UpdateAppMetadata(void);
void UpdateWindowSize(void);
void UpdateUniverseSize(void);
void UpdateResizable(void);
void UpdateFullscreen(void);
void UpdateMaximised(void);
void UpdateEnableLogs(void);
void UpdateLogDestination(void);

bool CallWrenCallback(int16 index, void *data, uint8 arg_count);

bool WrenGetVariable(const char *module, const char *variable);
const char *WrenGetString(const char *module, const char *variable, const char *field);
float64 WrenGetDouble(const char *module, const char *variable, const char *field);
int32 WrenGetInt(const char *module, const char *variable, const char *field);
bool WrenGetBool(const char *module, const char *variable, const char *field);
float64 WrenGetListDouble(const char *module, const char *variable, const char *field, int32 index);
int32 WrenGetListCount(const char *module, const char *variable, const char *field);
const char *WrenGetListString(const char *module, const char *variable, const char *field, int32 index);

void WriteFn(WrenVM *vm, const char *text);
void ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, int32 line, const char *msg);
WrenForeignMethodFn BindMethodFn(WrenVM *vm, const char *module, const char *className, bool isStatic,
                                 const char *signature);
WrenForeignClassMethods BindClassFn(WrenVM *vm, const char *module, const char *className);
WrenLoadModuleResult LoadModuleFn(WrenVM *vm, const char *name);
void InitBindingSystem();
void RegisterClass_(const char *module, const char *className, WrenForeignMethodFn allocateFn,
                    WrenFinalizerFn finalizeFn);
void RegisterMethod(const char *module, const char *className, bool isStatic, const char *signature,
                    WrenForeignMethodFn fn);

bool WrenLoadMainHandles(const char *main_module_name);
bool WrenCallOnStart();
bool WrenCallOnUpdate(float32 delta);
bool WrenCallOnFixedUpdate(float32 delta);
bool WrenCallOnRender();
bool WrenCallOnDestroy();
void WrenFree();

END_DECLARATIONS
