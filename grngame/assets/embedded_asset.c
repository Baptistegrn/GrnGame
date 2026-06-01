#include "embedded_asset.h"

#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTRY_MAX 200000
#define VAR_NAME_MAX 2048

static char g_hex_table[256][5];
static int g_hex_initialized = 0;

static void InitHexTable(void)
{
    if (g_hex_initialized)
        return;

    for (int i = 0; i < 256; ++i)
        snprintf(g_hex_table[i], sizeof(g_hex_table[i]), "0x%02X", i);

    g_hex_initialized = 1;
}

static int IsEmbeddableFile(const char *path)
{
    return FileIsLoadableScript(path) || FileIsLoadableAudio(path) || FileIsLoadableImage(path) ||
           FileIsLoadableText(path);
}

static void MakeVariableName(char *dst, uint64 dst_size, const char *path)
{
    snprintf(dst, dst_size, "embedded_%s", path);

    for (char *c = dst; *c; ++c)
    {
        switch (*c)
        {
        case '.':
        case '-':
        case ' ':
        case '/':
        case '\\':
            *c = '_';
            break;
        }
    }
}

static unsigned char *LoadFile(const char *path, long *out_size)
{
    FILE *file = fopen(path, "rb");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    *out_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (*out_size <= 0)
    {
        fclose(file);
        return NULL;
    }

    unsigned char *data = malloc(*out_size);

    if (!data)
    {
        fclose(file);
        return NULL;
    }

    fread(data, 1, *out_size, file);
    fclose(file);

    return data;
}

static void WriteEmbeddedArray(FILE *out, const unsigned char *data, long size, const char *var_name)
{
    fprintf(out, "static const unsigned char %s[] = {\n", var_name);

    for (long i = 0; i < size; ++i)
    {
        if ((i % 16) == 0)
            fprintf(out, "    ");

        fprintf(out, "%s", g_hex_table[data[i]]);

        if (i < size - 1)
            fprintf(out, ", ");

        if ((i % 16) == 15)
            fprintf(out, "\n");
    }

    fprintf(out, "\n};\n");
    fprintf(out, "#define %s_size %ld\n\n", var_name, size);
}

static void SerializeFile(FILE *out, const char *path, const char *var_name)
{
    long size = 0;
    unsigned char *data = LoadFile(path, &size);

    if (!data)
    {
        fprintf(out, "static const unsigned char %s[] = {\n", var_name);
        fprintf(out, "0\n};\n");
        fprintf(out, "#define %s_size 0\n\n", var_name);
        return;
    }

    WriteEmbeddedArray(out, data, size, var_name);

    free(data);
}

static void AppendRegistryEntry(EmbedContext *ctx, const char *path, const char *var_name)
{
    char entry[VAR_NAME_MAX];

    snprintf(entry, sizeof(entry), "    {\"%s\", %s, %s_size},\n", path, var_name, var_name);

    uint64 len = strlen(entry);

    if (ctx->registry_len + len + 1 >= REGISTRY_MAX)
        return;

    memcpy(ctx->registry + ctx->registry_len, entry, len);

    ctx->registry_len += len;
    ctx->registry[ctx->registry_len] = '\0';
}

static void EmbedCallback(const char *path, void *userdata)
{
    EmbedContext *ctx = userdata;

    if (!IsEmbeddableFile(path))
        return;

    char var_name[VAR_NAME_MAX];

    MakeVariableName(var_name, sizeof(var_name), path);

    SerializeFile(ctx->out, path, var_name);

    fprintf(ctx->out, "// asset: %s = %s\n\n", path, var_name);

    AppendRegistryEntry(ctx, path, var_name);
}

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header)
{
    InitHexTable();

    FILE *out = fopen(output_header, "w");

    if (!out)
    {
        fprintf(stderr, "[EmbeddedAsset] Failed to open output header: %s\n", output_header);
        return;
    }

    fprintf(out, "#pragma once\n\n");
    fprintf(out, "#include \"grngame/assets/asset_manager.h\"\n\n");

    char registry[REGISTRY_MAX] = {0};

    EmbedContext ctx = {.out = out, .registry = registry, .registry_len = 0};

    for (int i = 0; i < num_dirs; ++i)
        DirWalk(dirs[i], EmbedCallback, &ctx);

    fprintf(out, "static const EmbeddedAsset g_embedded_assets[] = {\n");

    fprintf(out, "%s", registry);

    fprintf(out, "    {0, 0, 0}\n"
                 "};\n\n");

    fclose(out);
}