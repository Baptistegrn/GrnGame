#include "db_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/data/data.h"
#include "grngame/utils/attributes.h"
#include <cstring>
#include <vector>

static void db_allocate(WrenVM *vm)
{
    sqlite3 **slot = (sqlite3 **)wrenSetSlotNewForeign(vm, 0, 0, sizeof(sqlite3 *));
    *slot = nullptr;
}

static void db_stmt_allocate(WrenVM *vm)
{
    DbStmt *stmt = (DbStmt *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(DbStmt));
    stmt->stmt = nullptr;
}

static void db_finalize(void *data)
{
    sqlite3 **db = (sqlite3 **)data;
    if (*db)
    {
        DbClose(*db);
        *db = nullptr;
    }
}

static void db_stmt_finalize(void *data)
{
    DbStmt *stmt = (DbStmt *)data;
    if (stmt->stmt)
        DbStmtFree(stmt);
}

static void db_open(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    *db = DbCreate(name);
}

static void db_close(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    if (*db)
    {
        DbClose(*db);
        *db = nullptr;
    }
}

static void db_exists(WrenVM *vm)
{
    const char *name = wren_get<const char *>(vm, 1);
    wren_set<bool>(vm, 0, DbExists(name));
}

static void db_fetch(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    const char *sql = wren_get<const char *>(vm, 1);
    DataFetchWren(*db, sql, vm);
}

static void db_write(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    const char *sql = wren_get<const char *>(vm, 1);
    wren_set<bool>(vm, 0, DataWrite(*db, sql));
}

static void db_begin(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    DbBegin(*db);
}

static void db_commit(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    DbCommit(*db);
}

static void db_rollback(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    DbRollback(*db);
}

static void db_prepare(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    const char *sql = wren_get<const char *>(vm, 1);

    DbStmt prepared = DbStmtPrepare(*db, sql);
    if (!prepared.stmt)
    {
        wrenSetSlotNull(vm, 0);
        return;
    }

    wrenEnsureSlots(vm, 2);
    wrenGetVariable(vm, "std/wren/data/db", "DbStmt", 1);
    DbStmt *stmt = (DbStmt *)wrenSetSlotNewForeign(vm, 0, 1, sizeof(DbStmt));
    *stmt = prepared;
}

static void db_stmt_run(WrenVM *vm)
{
    DbStmt *stmt = (DbStmt *)wrenGetSlotForeign(vm, 0);

    std::vector<DbArg> args;
    if (wrenGetSlotType(vm, 1) == WREN_TYPE_LIST)
    {
        int count = wrenGetListCount(vm, 1);
        args.reserve((size_t)count);

        for (int i = 0; i < count; i++)
        {
            wrenGetListElement(vm, 1, i, 2);
            DbArg arg = {0};

            switch (wrenGetSlotType(vm, 2))
            {
            case WREN_TYPE_BOOL:
                arg.type = INTEGER;
                arg.value.i = wrenGetSlotBool(vm, 2) ? 1 : 0;
                break;
            case WREN_TYPE_NUM: {
                double value = wrenGetSlotDouble(vm, 2);
                if (value == (double)(long long)value)
                {
                    arg.type = INTEGER;
                    arg.value.i = (int)value;
                }
                else
                {
                    arg.type = FLOAT;
                    arg.value.f = value;
                }
                break;
            }
            case WREN_TYPE_STRING:
                arg.type = TEXT;
                arg.value.s = wrenGetSlotString(vm, 2);
                break;
            case WREN_TYPE_NULL:
            default:
                arg.type = VOID;
                break;
            }

            args.push_back(arg);
        }
    }

    wren_set<bool>(vm, 0, DbStmtRun(stmt, args.empty() ? nullptr : args.data(), (int)args.size()));
}

static void db_stmt_free(WrenVM *vm)
{
    DbStmt *stmt = (DbStmt *)wrenGetSlotForeign(vm, 0);
    DbStmtFree(stmt);
}

WrenForeignClassMethods BindForeignClassCallbackDb(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    if (strcmp(module, "std/wren/data/db") == 0)
    {
        if (strcmp(class_name, "Db") == 0)
            return {db_allocate, db_finalize};
        if (strcmp(class_name, "DbStmt") == 0)
            return {db_stmt_allocate, db_stmt_finalize};
    }
    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackDb(WrenVM *vm, const char *module, const char *class_name, bool is_static,
                                                const char *signature)
{
    (void)vm;
    if (UNLIKELY(!module || !class_name || !signature))
        return nullptr;

    if (strcmp(module, "std/wren/data/db") == 0)
    {
        if (strcmp(class_name, "Db") == 0 && !is_static)
        {
            if (strcmp(signature, "open(_)") == 0)
                return db_open;
            if (strcmp(signature, "prepare(_)") == 0)
                return db_prepare;
            if (strcmp(signature, "close()") == 0)
                return db_close;
            if (strcmp(signature, "fetch(_)") == 0)
                return db_fetch;
            if (strcmp(signature, "write(_)") == 0)
                return db_write;
            if (strcmp(signature, "begin()") == 0)
                return db_begin;
            if (strcmp(signature, "commit()") == 0)
                return db_commit;
            if (strcmp(signature, "rollback()") == 0)
                return db_rollback;
        }
        if (strcmp(class_name, "Db") == 0 && is_static)
        {
            if (strcmp(signature, "exists(_)") == 0)
                return db_exists;
        }
        if (strcmp(class_name, "DbStmt") == 0 && !is_static)
        {
            if (strcmp(signature, "run(_)") == 0)
                return db_stmt_run;
            if (strcmp(signature, "free()") == 0)
                return db_stmt_free;
        }
    }

    return nullptr;
}