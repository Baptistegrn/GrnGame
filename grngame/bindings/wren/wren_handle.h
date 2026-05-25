#pragma once

bool WrenCallOnStart();
bool WrenCallOnUpdate(float delta);
bool WrenCallOnFixedUpdate(float delta);
bool WrenCallOnRender();
bool WrenCallOnDestroy();
void WrenFree();

bool WrenLoadMainHandles(const char *main_module_name);