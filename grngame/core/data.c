#include "grngame/core/app.h"
#include "grngame/data/file.h"
#include "grngame/data/sqlite.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/version.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

static sqlite3 *OpenStorageDb(const char *name)
{
    char *path = PathFromStorage(name);
    if (!path)
        return NULL;

    sqlite3 *db = DbCreate(path);
    free(path);
    return db;
}

typedef enum
{
    ASSETS_PAK_INSTALL,
    ASSETS_PAK_REPLACE,
    ASSETS_PAK_KEEP_INSTALLED,
    ASSETS_PAK_ERROR
} AssetsPakDecision;

static AssetsPakDecision DecideAssetsPakAction(const char *installed_path, const char *bundled_path)
{
    if (!FileExist(installed_path))
    {
        LOG_DEBUG("Assets.pak not present in storage move it");
        return ASSETS_PAK_INSTALL;
    }

    int32 installed_version = 0;
    int32 bundled_version = 0;

    if (!ReadSqliteVersion(installed_path, &installed_version) || !ReadSqliteVersion(bundled_path, &bundled_version))
    {
        LOG_ERROR("Cannot read Assets.pak versions");
        return ASSETS_PAK_ERROR;
    }

    if (bundled_version > installed_version)
    {
        // parse should work
        char old[128];
        ParseVersionToString(installed_version, old, sizeof(old));
        char new[128];
        ParseVersionToString(installed_version, new, sizeof(new));

        LOG_DEBUG("Changing assets.pak version from %s to %s", old, new);
        return ASSETS_PAK_REPLACE;
    }
    else
    {
        return ASSETS_PAK_KEEP_INSTALLED;
    }
}

static bool ExecuteAssetsPakDecision(AssetsPakDecision decision, const char *installed_path, const char *bundled_path)
{
    switch (decision)
    {
    case ASSETS_PAK_KEEP_INSTALLED:
        return true;

    case ASSETS_PAK_INSTALL:
        return MoveFile_(installed_path, bundled_path);

    case ASSETS_PAK_REPLACE:
        return RemovePath(installed_path) && MoveFile_(installed_path, bundled_path);

    case ASSETS_PAK_ERROR:
    default:
        return false;
    }
}

bool InstallAssetsPak(void)
{
#ifdef GRNGAME_WASM
    return true;
#else
    char *installed_path = PathFromStorage("Assets.pak");
    char *bundled_path = PathFromExecutableDirectory("Assets.pak");

    bool ok = false;
    if (installed_path && bundled_path)
    {
        AssetsPakDecision decision = DecideAssetsPakAction(installed_path, bundled_path);
        ok = ExecuteAssetsPakDecision(decision, installed_path, bundled_path);
    }

    free(installed_path);
    free(bundled_path);
    return ok;
#endif
}

static bool CreateGameDataFile(const char *path, const char *version)
{
    sqlite3 *db = DbCreate(path);
    if (!db)
        return false;

    bool ok = ApplyDbPragmas(db) && ApplyDbVersion(db, version) &&
              DataWrite(db, "CREATE TABLE IF NOT EXISTS data (path TEXT PRIMARY KEY, content BLOB);");

    DbClose(db);

    if (!ok)
        RemovePath(path);
    return ok;
}

bool InstallGameData(void)
{
    char *path = PathFromStorage("game.data");
    if (!path)
        return false;

    bool ok = true;
    if (!FileExist(path))
    {
        LOG_DEBUG("Game.data does not exist, creating");
        ok = CreateGameDataFile(path, g_app.info.version);
    }

    free(path);
    return ok;
}

bool InitAssetsPak(void)
{
    g_app.info.file_db = OpenStorageDb("Assets.pak");
    if (!g_app.info.file_db)
    {
        LOG_ERROR("Cannot open Assets.pak");
        return false;
    }
    return true;
}

static bool DetectGameDataVersionChange(const char *path)
{
    int32 app_version = 0;
    if (!ParseVersionToInt(g_app.info.version, &app_version))
    {
        LOG_ERROR("Cannot parse application version");
        return false;
    }

    int32 stored_version = 0;
    if (!ReadSqliteVersion(path, &stored_version))
    {
        LOG_ERROR("Cannot read game.data version");
        return false;
    }

    bool version_changed = (stored_version != app_version);

    if (version_changed)
    {
        char stored_str[64];
        if (!ParseVersionToString(stored_version, stored_str, sizeof(stored_str)))
        {
            LOG_ERROR("Cannot read game.data version");
            return false;
        }

        LOG_DEBUG("Changing game.data version from %s to %s", stored_str, g_app.info.version);
        g_app.info.old_version = strdup(stored_str);
    }

    g_app.info.update_event_data = version_changed;
    return true;
}

bool InitGameData(void)
{
    char *path = PathFromStorage("game.data");
    if (!path)
        return false;

    bool ok = DetectGameDataVersionChange(path);
    free(path);
    if (!ok)
        return false;

    g_app.info.data_db = OpenStorageDb("game.data");
    if (!g_app.info.data_db)
    {
        LOG_ERROR("Cannot open game.data");
        return false;
    }

    return ApplyDbPragmas(g_app.info.data_db);
}