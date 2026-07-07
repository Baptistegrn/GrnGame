#include "asset_manager.h"
#include "SDL3/SDL_error.h"
#include "grngame/assets/load.h"
#include "grngame/core/app.h"
#include "grngame/data/data.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/string_compat.h"
#include "khash.h"
#include "kvec.h"

static COLD void LoadFile(const char *file, void *user_data);
static COLD void LoadEmbeddedFiles();

COLD AssetManager AssetManagerCreate()
{
    return (AssetManager){.sound_map = kh_init(SoundMap), .texture_map = kh_init(TextureMap)};
}

COLD void AssetManagerLoadFolder(const char *folder)
{

#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        if (UNLIKELY(g_app.embedded_assets_count == 0))
        {
            LOG_WARNING("No assets files in embedded assets folder '%s'", folder);
            return;
        }
        LoadEmbeddedFiles();
    }

#else
    {
        if (DirAssetFileCount(folder) == 0)
        {
            LOG_WARNING("No assets files in asset folder '%s'", folder);
            return;
        }

        DirWalk(folder, LoadFile, NULL);
    }
#endif
}

static COLD void LoadFile(const char *file, void *user_data)
{
    (void)user_data;

    bool load_result = false;

    if (FileIsLoadableAudio(file))
    {
        load_result = LoadSoundFile(file);
        if (!load_result)
            LOG_WARNING("Failed to load asset file '%s'", file);
        else
            LOG_DEBUG("Loaded asset file '%s'", file);
    }
    else if (FileIsLoadableImage(file))
    {
        load_result = LoadTextureFile(file);
        if (!load_result)
            LOG_WARNING("Failed to load asset file '%s', SDL error: '%s'", file, SDL_GetError());
        else
            LOG_DEBUG("Loaded asset file '%s'", file);
    }
    else
    {
        LOG_WARNING("Unknown file type in asset folder: '%s'", FileExtension(file));
    }
}

static COLD bool LoadEmbeddedFile(const char *name)
{
    if (!name)
        return false;

    if (FileIsLoadableAudio(name))
    {
        bool result = LoadSoundFile(name);
        if (result)
            LOG_DEBUG("Loaded asset file '%s' (embedded)", name);
        else
            LOG_WARNING("Failed to load asset file '%s' (embedded)", name);
        return result;
    }
    else if (FileIsLoadableImage(name))
    {
        bool result = LoadTextureFile(name);
        if (result)
            LOG_DEBUG("Loaded asset file '%s' (embedded)", name);
        else
            LOG_WARNING("Failed to load asset file '%s' (embedded), SDL error: '%s'", name, SDL_GetError());
        return result;
    }
    return false;
}

static COLD void LoadEmbeddedFiles()
{
    khash_t(EmbeddedAssetHash) *hash = &g_app.embedded_assets_hash;
    for (khint_t k = kh_begin(hash); k != kh_end(hash); ++k)
    {
        if (kh_exist(hash, k))
        {
            EmbeddedAsset asset = kh_value(hash, k);
            LoadEmbeddedFile(asset.name);
        }
    }
}
static COLD int32 EmbeddedFileCountAssets(sqlite3 *db)
{
    int32 count = 0;
    DbResult res = DataFetch(db, "SELECT value FROM embedded_assets_info WHERE key = 'asset_count';");

    if (kv_size(res.rows) > 0)
    {
        DbRow row = kv_A(res.rows, 0);
        if (kv_size(row.cols) > 0 && kv_A(row.cols, 0).type == INTEGER)
        {
            count = kv_A(row.cols, 0).value.i;
        }
    }

    DbResultFree(&res);
    return count;
}

static COLD int32 EmbeddedFileCount(sqlite3 *db)
{
    int32 count = 0;
    DbResult res = DataFetch(db, "SELECT value FROM embedded_assets_info WHERE key = 'file_count';");

    if (kv_size(res.rows) > 0)
    {
        DbRow row = kv_A(res.rows, 0);
        if (kv_size(row.cols) > 0 && kv_A(row.cols, 0).type == INTEGER)
        {
            count = kv_A(row.cols, 0).value.i;
        }
    }

    DbResultFree(&res);
    return count;
}

COLD void CreateEmbeddedAssetsCache(sqlite3 *db)
{
    sqlite3_stmt *stmt = NULL;

    g_app.embedded_assets_count = EmbeddedFileCountAssets(db);
    g_app.embedded_count = EmbeddedFileCount(db);
    if (sqlite3_prepare_v2(db, "SELECT path, data FROM embedded_assets;", -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR("Failed to init asset cache: %s", sqlite3_errmsg(db));
        return;
    }

    khash_t(EmbeddedAssetHash) *hash = &g_app.embedded_assets_hash;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *path = (const char *)sqlite3_column_text(stmt, 0);
        int32 size = sqlite3_column_bytes(stmt, 1);
        const void *blob = sqlite3_column_blob(stmt, 1);

        EmbeddedAsset asset;
        asset.name = strdup(path);
        asset.size = size;
        asset.data = malloc(size);

        if (asset.data && blob)
        {
            memcpy((void *)asset.data, blob, size);
        }

        int32 ret;
        khiter_t k = kh_put(EmbeddedAssetHash, hash, asset.name, &ret);

        if (UNLIKELY(ret == 0))
        {
            free((char *)kh_key(hash, k));
            kh_key(hash, k) = asset.name;
        }

        kh_value(hash, k) = asset;
    }

    sqlite3_finalize(stmt);
    DbClose(g_app.info.asset_db);
}