#include "input_module.hpp"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"

#include "daScript/simulate/simulate.h"
#include "grngame/input/controller.h"
#include "grngame/input/keyboard.h"
#include "grngame/input/mouse.h"

InputModule::InputModule() : das::Module("grngame_input")
{
    auto module_library = das::ModuleLibrary(this);
    module_library.addBuiltInModule();
    module_library.addModule(this);

    // Keyboard
    das::addExtern<DAS_BIND_FUN(KeyPressed)>(*this, module_library, "key_pressed", das::SideEffects::none,
                                             "KeyPressed");
    das::addExtern<DAS_BIND_FUN(KeyJustPressed)>(*this, module_library, "key_just_pressed", das::SideEffects::none,
                                                 "KeyJustPressed");

    // Mouse position
    das::addExtern<DAS_BIND_FUN(GetMousePosition), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, module_library, "get_mouse_position", das::SideEffects::none,
                                                   "GetMousePosition");
    das::addExtern<DAS_BIND_FUN(MouseX)>(*this, module_library, "mouse_x", das::SideEffects::none,
                                         "MouseX");
    das::addExtern<DAS_BIND_FUN(MouseY)>(*this, module_library, "mouse_y", das::SideEffects::none,
                                         "MouseY");

    // Mouse buttons
    das::addExtern<DAS_BIND_FUN(MouseLeftPressed)>(*this, module_library, "mouse_left_pressed", das::SideEffects::none,
                                                   "MouseLeftPressed");
    das::addExtern<DAS_BIND_FUN(MouseLeftJustPressed)>(*this, module_library, "mouse_left_just_pressed", das::SideEffects::none,
                                                       "MouseLeftJustPressed");
    das::addExtern<DAS_BIND_FUN(MouseRightPressed)>(*this, module_library, "mouse_right_pressed", das::SideEffects::none,
                                                    "MouseRightPressed");
    das::addExtern<DAS_BIND_FUN(MouseRightJustPressed)>(*this, module_library, "mouse_right_just_pressed", das::SideEffects::none,
                                                        "MouseRightJustPressed");

    // Mouse scroll
    das::addExtern<DAS_BIND_FUN(MouseScrollX)>(*this, module_library, "mouse_scroll_x", das::SideEffects::none,
                                               "MouseScrollX");
    das::addExtern<DAS_BIND_FUN(MouseScrollY)>(*this, module_library, "mouse_scroll_y", das::SideEffects::none,
                                               "MouseScrollY");

    // Mouse control
    das::addExtern<DAS_BIND_FUN(MoveMouse)>(*this, module_library, "move_mouse", das::SideEffects::modifyExternal,
                                            "MoveMouse");
    das::addExtern<DAS_BIND_FUN(ShowCursor)>(*this, module_library, "show_cursor", das::SideEffects::modifyExternal,
                                             "ShowCursor");
    das::addExtern<DAS_BIND_FUN(HideCursor)>(*this, module_library, "hide_cursor", das::SideEffects::modifyExternal,
                                             "HideCursor");
}
REGISTER_MODULE(InputModule);