/*
 * Utilitaires Lua et wrapper C++ pour les sons.
 * Gere la conversion entre les types Sol2 et les types C du moteur.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* joueur un son par lien */
void lua_play_sound(const std::string &path, sol::optional<int> loop, sol::optional<int> channel,
                    sol::optional<int> volume) {
    int l = loop.value_or(DEFAULT_LOOP);
    int c = channel.value_or(DEFAULT_CHANNEL);
    int v = volume.value_or(DEFAULT_VOLUME);
    playSound(path.c_str(), l, c, v);
}

/* coupe un son par lien */
void lua_stop_sound(const std::string &path) { stopSound(path.c_str()); }

/* reprendre un son par lien */
void lua_resume_sound(const std::string &path) { resumeSound(path.c_str()); }

/* charger un dossier de son */
void lua_load_song_folder(const std::string &path) { loadSongFolder(path.c_str()); }

/* enregistrement des bindings son */
void enregistrer_bindings_son(sol::table &song) {
    song.set_function("playSound", &lua_play_sound);
    song.set_function("stopSound", &lua_stop_sound);
    song.set_function("resumeSound", &lua_resume_sound);
    song.set_function("loadSongFolder", &lua_load_song_folder);
    song.set_function("stopChannel", &stopChannel);
    song.set_function("pauseChannel", pauseChannel);
    song.set_function("resumeChannel", &resumeChannel);
    song.set_function("freeSongFolder", &freeSongFolder);
}