// #include <stdio.h>

// #include <lauxlib.h>
// #include <lua.h>
// #include <lualib.h>

// #define GAME_EXPORTS
// #include "../bindings_c/GrnGame.h"

// // Variables globales internes pour gérer l'état Lua et le callback
// static lua_State *etat_lua = NULL;
// static int callack_fonction = LUA_NOREF;

// /*
//  * Wrapper interne appelé par le moteur C pour exécuter la fonction Update Lua.
//  * Récupère la fonction Lua depuis le registre et l'appelle sans arguments.
//  */
// void mise_a_jour_wrapper() {
//     if (etat_lua == NULL || callack_fonction == LUA_NOREF) {
//         return;
//     }

//     // Récupération de la fonction Lua
//     lua_rawgeti(etat_lua, LUA_REGISTRYINDEX, callack_fonction);

//     // Vérification que c'est bien une fonction
//     if (!lua_isfunction(etat_lua, -1)) {
//         lua_pop(etat_lua, 1);
//         return;
//     }

//     // Appel de la fonction Lua
//     if (lua_pcall(etat_lua, 0, 0, 0) != LUA_OK) {
//         const char *msg = lua_tostring(etat_lua, -1);
//         fprintf(stderr, "%s\n", msg);
//         lua_pop(etat_lua, 1);
//     }
// }

// static int l_initialize(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 8) {
//         return luaL_error(L, "initialize expects 8 arguments (received %d)", n);
//     }

//     int height = (int)luaL_checkinteger(L, 1);
//     int width = (int)luaL_checkinteger(L, 2);
//     float fps_target = (float)luaL_checknumber(L, 3);
//     int scale_coeff = (int)luaL_checkinteger(L, 4);
//     int black_bars = (int)luaL_checkinteger(L, 5);

//     const char *title = luaL_checkstring(L, 6);

//     const char *log_path = luaL_checkstring(L, 7);

//     // Gestion du callback
//     if (lua_type(L, 8) != LUA_TFUNCTION) {
//         return luaL_error(L, "initialize: argument 8 must be a function");
//     }

//     // Sauvegarde de la fonction Lua
//     if (callack_fonction != LUA_NOREF) {
//         luaL_unref(L, LUA_REGISTRYINDEX, callack_fonction);
//     }

//     etat_lua = L;
//     lua_pushvalue(L, 8);
//     callack_fonction = luaL_ref(L, LUA_REGISTRYINDEX);

//     initialize(height, width, fps_target, scale_coeff, black_bars, title, log_path,
//                mise_a_jour_wrapper);

//     return 0;
// }

// static int l_stop(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 0) {
//         return luaL_error(L, "stop expects 0 arguments (received %d)", n);
//     }
//     stop();
//     return 0;
// }

// static int l_toggleFullScreen(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 0) {
//         return luaL_error(L, "toggleFullscreen expects 0 arguments (received %d)", n);
//     }
//     toggleFullscreen();
//     return 0;
// }

// static int l_clearScreen(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 3) {
//         return luaL_error(L, "clearScreen expects 3 arguments (received %d)", n);
//     }

//     int red = (int)luaL_checkinteger(L, 1);
//     int green = (int)luaL_checkinteger(L, 2);
//     int blue = (int)luaL_checkinteger(L, 3);

//     clearScreen(red, green, blue);
//     return 0;
// }

// static int l_setLogLevel(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1) {
//         return luaL_error(L, "setLogLvl expects 1 argument (received %d)", n);
//     }

//     int level = (int)luaL_checkinteger(L, 1);
//     setLogLvl(level);
//     return 0;
// }

// static int l_log_message(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 2) {
//         return luaL_error(L, "logMessage expects 2 arguments (received %d)", n);
//     }

//     int level = (int)luaL_checkinteger(L, 1);
//     const char *message = luaL_checkstring(L, 2);

//     logMessage(level, (char *)message);
//     return 0;
// }

// static int l_key_just_pressed(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1) {
//         return luaL_error(L, "keyJustPressed expects 1 argument (received %d)", n);
//     }

//     const char *key = luaL_checkstring(L, 1);
//     bool pressed = keyJustPressed(key);
//     lua_pushboolean(L, pressed);
//     return 1;
// }

// static int l_key_pressed(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1) {
//         return luaL_error(L, "keyPressed expects 1 argument (received %d)", n);
//     }

//     const char *key = luaL_checkstring(L, 1);
//     bool pressed = keyPressed(key);
//     lua_pushboolean(L, pressed);
//     return 1;
// }

// static int l_mouse_x(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 0) {
//         return luaL_error(L, "mouseX expects 0 arguments (received %d)", n);
//     }
//     lua_pushinteger(L, mouseX());
//     return 1;
// }

// static int l_mouse_y(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 0) {
//         return luaL_error(L, "mouseY expects 0 arguments (received %d)", n);
//     }
//     lua_pushinteger(L, mouseY());
//     return 1;
// }

// static int l_mouse_left_just_pressed(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "mouseLeftJustPressed expects 0 arguments");
//     lua_pushboolean(L, mouseLeftJustPressed());
//     return 1;
// }

// static int l_mouse_right_just_pressed(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "mouseRightJustPressed expects 0 arguments");
//     lua_pushboolean(L, mouseRightJustPressed());
//     return 1;
// }

// static int l_mouse_left_pressed(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "mouseLeftPressed expects 0 arguments");
//     lua_pushboolean(L, mouseLeftPressed());
//     return 1;
// }

// static int l_mouse_right_pressed(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "mouseRightPressed expects 0 arguments");
//     lua_pushboolean(L, mouseRightPressed());
//     return 1;
// }

// static int l_delta(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "delta expects 0 arguments");
//     lua_pushnumber(L, delta());
//     return 1;
// }

// static int l_fps(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "fps expects 0 arguments");
//     lua_pushnumber(L, fps());
//     return 1;
// }

// static int l_frame_count(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "frameCount expects 0 arguments");
//     lua_pushinteger(L, (lua_Integer)frameCount());
//     return 1;
// }

// static int l_offset_x(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "offsetX expects 0 arguments");
//     lua_pushinteger(L, offsetX());
//     return 1;
// }

// static int l_offset_y(lua_State *L) {
//     if (lua_gettop(L) != 0)
//         return luaL_error(L, "offsetY expects 0 arguments");
//     lua_pushinteger(L, offsetY());
//     return 1;
// }

// static int l_draw(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 8) {
//         return luaL_error(L, "draw expects 8 arguments (received %d)", n);
//     }
//     const char *id = luaL_checkstring(L, 1);
//     float x = (float)luaL_checknumber(L, 2);
//     float y = (float)luaL_checknumber(L, 3);
//     Sint16 width = (Sint16)luaL_checkinteger(L, 4);
//     Sint16 height = (Sint16)luaL_checkinteger(L, 5);
//     bool flip = lua_toboolean(L, 6);
//     Uint16 rotation = (Uint16)luaL_checkinteger(L, 7);
//     Uint8 alpha = (Uint8)luaL_checkinteger(L, 8);
//     draw(id, x, y, width, height, flip, rotation, alpha);
//     return 0;
// }

// static int l_draw_text(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 9) {
//         return luaL_error(L, "drawText expects 9 arguments (received %d)", n);
//     }

//     const char *font_path = luaL_checkstring(L, 1);
//     const char *text = luaL_checkstring(L, 2);

//     float x = (float)luaL_checknumber(L, 3);
//     float y = (float)luaL_checknumber(L, 4);

//     Uint16 scale = (Uint16)luaL_checkinteger(L, 5);
//     bool flip = lua_toboolean(L, 6);

//     float spacing = (float)luaL_checknumber(L, 7);

//     Uint16 rotation = (Uint16)luaL_checkinteger(L, 8);
//     Uint8 alpha = (Uint8)luaL_checkinteger(L, 9);

//     drawText(font_path, text, x, y, scale, flip, spacing, rotation, alpha);
//     return 0;
// }

// static int l_load_image_folder(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1) {
//         return luaL_error(L, "loadImageFolder expects 1 arguments (received %d)", n);
//     }
//     const char *path = luaL_checkstring(L, 1);
//     loadImageFolder(path);
//     return 0;
// }

// static int l_free_image_folder(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 0) {
//         return luaL_error(L, "freeImageFolder expects 0 arguments (received %d)", n);
//     }
//     freeImageFolder();
//     return 0;
// }

// static int l_set_icon(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1) {
//         return luaL_error(L, "setIcon expects 1 arguments (received %d)", n);
//     }
//     const char *path = luaL_checkstring(L, 1);
//     setIcon(path);
//     return 0;
// }

// static int l_draw_rect(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 9)
//         return luaL_error(L, "drawRect expects 9 arguments (received %d)", n);

//     float x = (float)luaL_checknumber(L, 1);
//     float y = (float)luaL_checknumber(L, 2);
//     Sint16 width = (Sint16)luaL_checkinteger(L, 3);
//     Sint16 height = (Sint16)luaL_checkinteger(L, 4);

//     Uint8 red = (Uint8)luaL_checkinteger(L, 5);
//     Uint8 green = (Uint8)luaL_checkinteger(L, 6);
//     Uint8 blue = (Uint8)luaL_checkinteger(L, 7);
//     Uint8 alpha = (Uint8)luaL_checkinteger(L, 8);

//     bool filled = lua_toboolean(L, 9);

//     drawRect(x, y, width, height, red, green, blue, alpha, filled);
//     return 0;
// }

// static int l_draw_circle(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 7)
//         return luaL_error(L, "drawCircle expects 7 arguments (received %d)", n);

//     float x = (float)luaL_checknumber(L, 1);
//     float y = (float)luaL_checknumber(L, 2);
//     Sint16 radius = (Sint16)luaL_checkinteger(L, 3);

//     Uint8 red = (Uint8)luaL_checkinteger(L, 4);
//     Uint8 green = (Uint8)luaL_checkinteger(L, 5);
//     Uint8 blue = (Uint8)luaL_checkinteger(L, 6);

//     bool filled = lua_toboolean(L, 7);

//     drawCircle(x, y, radius, red, green, blue, filled);
//     return 0;
// }

// static int l_draw_triangle(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 7)
//         return luaL_error(L, "drawTriangle expects 7 arguments (received %d)", n);

//     float x = (float)luaL_checknumber(L, 1);
//     float y = (float)luaL_checknumber(L, 2);
//     Sint16 w = (Sint16)luaL_checkinteger(L, 3);
//     Sint16 h = (Sint16)luaL_checkinteger(L, 4);

//     Uint8 red = (Uint8)luaL_checkinteger(L, 5);
//     Uint8 green = (Uint8)luaL_checkinteger(L, 6);
//     Uint8 blue = (Uint8)luaL_checkinteger(L, 7);

//     bool filled = lua_toboolean(L, 8); // ← attention ici
//     drawTriangle(x, y, w, h, red, green, blue, filled);
//     return 0;
// }

// static int l_draw_line(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 7)
//         return luaL_error(L, "drawLine expects 7 arguments (received %d)", n);

//     float x1 = (float)luaL_checknumber(L, 1);
//     float y1 = (float)luaL_checknumber(L, 2);
//     float x2 = (float)luaL_checknumber(L, 3);
//     float y2 = (float)luaL_checknumber(L, 4);

//     Uint8 red = (Uint8)luaL_checkinteger(L, 5);
//     Uint8 green = (Uint8)luaL_checkinteger(L, 6);
//     Uint8 blue = (Uint8)luaL_checkinteger(L, 7);

//     drawLine(x1, y1, x2, y2, red, green, blue);
//     return 0;
// }

// static int l_button_pressed(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 2)
//         return luaL_error(L, "buttonPressed expects 2 arguments (received %d)", n);

//     const char *button_name = luaL_checkstring(L, 1);
//     unsigned char index = (unsigned char)luaL_checkinteger(L, 2);

//     int result = buttonPressed(button_name, index);
//     lua_pushinteger(L, result);
//     return 1;
// }

// static int l_button_just_pressed(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 2)
//         return luaL_error(L, "buttonJustPressed expects 2 arguments (received %d)", n);

//     const char *button_name = luaL_checkstring(L, 1);
//     unsigned char index = (unsigned char)luaL_checkinteger(L, 2);

//     int result = buttonJustPressed(button_name, index);
//     lua_pushinteger(L, result);
//     return 1;
// }

// static int l_init_controller(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1)
//         return luaL_error(L, "initController expects 1 argument (received %d)", n);

//     unsigned char index = (unsigned char)luaL_checkinteger(L, 1);
//     initController(index);
//     return 0;
// }

// // pour linstant renvoie un tableau de 6 floats
// // on pourrait plus tard faire une classe lua
// static int l_get_joysticks(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 2)
//         return luaL_error(L, "getJoysticks expects 2 arguments (received %d)", n);

//     float dead_zone = (float)luaL_checknumber(L, 1);
//     unsigned char index = (unsigned char)luaL_checkinteger(L, 2);

//     float *axes = getJoysticks(dead_zone, index);

//     for (int i = 0; i < 6; ++i)
//         lua_pushnumber(L, axes[i]);

//     return 6;
// }

// static int l_close_the_controller(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 1)
//         return luaL_error(L, "closeTheController expects 1 argument (received %d)", n);

//     unsigned char index = (unsigned char)luaL_checkinteger(L, 1);
//     closeTheController(index);
//     return 0;
// }

// static int l_get_blocks_from_file(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 5) {
//         return luaL_error(L, "getBlocksFromFile expects 5 arguments (received %d)", n);
//     }
//     const char *chemin = luaL_checkstring(L, 1);
//     Uint8 pas_x = (Uint8)luaL_checkinteger(L, 2);
//     Uint8 pas_y = (Uint8)luaL_checkinteger(L, 3);
//     char separation = (char)luaL_checkinteger(L, 4);
//     int excludeElement = (int)luaL_checkinteger(L, 5);
//     Blocks *blocs = getBlocksFromFile(chemin, pas_x, pas_y, separation, excludeElement);
//     if (blocs == NULL) {
//         lua_pushnil(L);
//         return 1;
//     }
//     Blocks **ud = (Blocks **)lua_newuserdata(L, sizeof(Blocks *));
//     *ud = blocs;
//     luaL_getmetatable(L, "BlocksMeta");
//     lua_setmetatable(L, -2);
//     return 1;
// }
// static int l_platformer(lua_State *L) {
//     int n = lua_gettop(L);
//     if (n != 4) {
//         return luaL_error(L, "hitboxPlatformer expects 4 arguments (received %d)", n);
//     }
//     EntityPlatformer **ent_ptr = (EntityPlatformer **)luaL_checkudata(L, 1, "EntityMeta");
//     EntityPlatformer *entity = *ent_ptr;

//     Blocks **blk_ptr = (Blocks **)luaL_checkudata(L, 2, "BlocksMeta");
//     Blocks *blocks = *blk_ptr;

//     float vitesse = (float)luaL_checknumber(L, 3);
//     float correction = (float)luaL_checknumber(L, 4);
//     EntityPlatformer *result = hitboxPlatformer(entity, blocks, vitesse, correction);
//     lua_pushboolean(L, 1);
//     return 1;
// }

// // Enregistrement des bindings Lua
// void enregistrer_bindings(lua_State *L) {
//     // global
//     lua_register(L, "initialize", l_initialize);
//     lua_register(L, "stop", l_stop);
//     lua_register(L, "toggleFullscreen", l_toggleFullScreen);
//     lua_register(L, "clearScreen", l_clearScreen);
//     lua_register(L, "setLogLvl", l_setLogLevel);
//     lua_register(L, "logMessage", l_log_message);
//     // input
//     lua_newtable(L);

//     lua_pushcfunction(L, l_key_just_pressed);
//     lua_setfield(L, -2, "keyJustPressed");
//     lua_pushcfunction(L, l_key_pressed);
//     lua_setfield(L, -2, "keyPressed");
//     lua_pushcfunction(L, l_mouse_x);
//     lua_setfield(L, -2, "mouseX");
//     lua_pushcfunction(L, l_mouse_y);
//     lua_setfield(L, -2, "mouseY");
//     lua_pushcfunction(L, l_mouse_left_just_pressed);
//     lua_setfield(L, -2, "mouseLeftJustPressed");
//     lua_pushcfunction(L, l_mouse_right_just_pressed);
//     lua_setfield(L, -2, "mouseRightJustPressed");
//     lua_pushcfunction(L, l_mouse_left_pressed);
//     lua_setfield(L, -2, "mouseLeftPressed");
//     lua_pushcfunction(L, l_mouse_right_pressed);
//     lua_setfield(L, -2, "mouseRightPressed");
//     lua_pushcfunction(L, l_button_pressed);
//     lua_setfield(L, -2, "buttonPressed");
//     lua_pushcfunction(L, l_button_just_pressed);
//     lua_setfield(L, -2, "buttonJustPressed");
//     lua_pushcfunction(L, l_init_controller);
//     lua_setfield(L, -2, "initController");
//     lua_pushcfunction(L, l_get_joysticks);
//     lua_setfield(L, -2, "getJoysticks");
//     lua_pushcfunction(L, l_close_the_controller);
//     lua_setfield(L, -2, "closeTheController");
//     lua_setglobal(L, "Input");

//     // const
//     lua_newtable(L);

//     lua_pushcfunction(L, l_delta);
//     lua_setfield(L, -2, "delta");
//     lua_pushcfunction(L, l_fps);
//     lua_setfield(L, -2, "fps");
//     lua_pushcfunction(L, l_frame_count);
//     lua_setfield(L, -2, "frameCount");
//     lua_pushcfunction(L, l_offset_x);
//     lua_setfield(L, -2, "offsetX");
//     lua_pushcfunction(L, l_offset_y);
//     lua_setfield(L, -2, "offsetY");

//     lua_setglobal(L, "Const");

//     // image
//     lua_newtable(L);
//     lua_pushcfunction(L, l_draw);
//     lua_setfield(L, -2, "draw");
//     lua_pushcfunction(L, l_draw_text);
//     lua_setfield(L, -2, "drawText");
//     lua_pushcfunction(L, l_load_image_folder);
//     lua_setfield(L, -2, "loadImageFolder");
//     lua_pushcfunction(L, l_free_image_folder);
//     lua_setfield(L, -2, "freeImageFolder");
//     lua_pushcfunction(L, l_set_icon);
//     lua_setfield(L, -2, "setIcon");
//     lua_pushcfunction(L, l_draw_rect);
//     lua_setfield(L, -2, "drawRect");
//     lua_pushcfunction(L, l_draw_circle);
//     lua_setfield(L, -2, "drawCircle");
//     lua_pushcfunction(L, l_draw_triangle);
//     lua_setfield(L, -2, "drawTriangle");
//     lua_pushcfunction(L, l_draw_line);
//     lua_setfield(L, -2, "drawLine");
//     lua_setglobal(L, "Image");
// }