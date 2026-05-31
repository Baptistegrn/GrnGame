#include "embedded_asset.h"
#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTRY_MAX 200000

static char hex_table[256][5];
static int hex_init = 0;

static void init_hex_table(void)
{
    if (hex_init)
        return;
    for (int i = 0; i < 256; i++)
        snprintf(hex_table[i], 5, "0x%02X", i);
    hex_init = 1;
}

static void serialize_file(FILE *out, const char *path, const char *var_name)
{
    FILE *fs = fopen(path, "rb");
    if (!fs)
        return;

    fseek(fs, 0, SEEK_END);
    long size = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    fprintf(out, "static const unsigned char %s[] = {\n", var_name);

    if (size == 0)
    {
        fprintf(out, "0\n};\n#define %s_size 0\n\n", var_name);
        fclose(fs);
        return;
    }

    unsigned char *data = malloc(size);
    fread(data, 1, size, fs);
    fclose(fs);

    size_t buf_size = size * 7 + (size / 16) * 8 + 256;
    char *buf = malloc(buf_size);
    char *p = buf;

    for (long i = 0; i < size; i++)
    {
        if (i % 16 == 0)
        {
            *p++ = ' ';
            *p++ = ' ';
            *p++ = ' ';
            *p++ = ' ';
        }

        const char *h = hex_table[data[i]];
        *p++ = h[0];
        *p++ = h[1];
        *p++ = h[2];
        *p++ = h[3];

        if (i < size - 1)
        {
            *p++ = ',';
            *p++ = ' ';
        }

        if (i % 16 == 15)
            *p++ = '\n';
    }
    *p++ = '\n';

    fwrite(buf, 1, p - buf, out);
    fprintf(out, "};\n#define %s_size %ld\n\n", var_name, size);

    free(buf);
    free(data);
}

static void embed_callback(const char *path, void *userdata)
{
    EmbedContext *ctx = (EmbedContext *)userdata;

    if (!FileIsLoadableScript(path) && !FileIsLoadableAudio(path) && !FileIsLoadableImage(path) &&
        !FileIsLoadableText(path))
        return;

    char var_name[2048];
    snprintf(var_name, sizeof(var_name), "embedded_%s", path);
    for (char *c = var_name; *c; c++)
    {
        if (*c == '.' || *c == '-' || *c == ' ' || *c == '/' || *c == '\\')
            *c = '_';
    }

    serialize_file(ctx->out, path, var_name);

    fprintf(ctx->out, "// asset: %s = %s\n\n", path, var_name);

    char entry[2048];
    snprintf(entry, sizeof(entry), "    {\"%s\", %s, %s_size},\n", path, var_name, var_name);

    size_t entry_len = strlen(entry);
    if (ctx->registry_len + entry_len + 1 < REGISTRY_MAX)
    {
        memcpy(ctx->registry + ctx->registry_len, entry, entry_len);
        ctx->registry_len += entry_len;
        ctx->registry[ctx->registry_len] = '\0';
    }
}

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header)
{
    init_hex_table();

    FILE *out = fopen(output_header, "w");
    if (!out)
    {
        fprintf(stderr, "[EmbeddedAsset] Failed to open output header: %s\n", output_header);
        return;
    }

    fprintf(out, "#pragma once\n\n");
    fprintf(out, "#include \"grngame/assets/asset_manager.h\"\n\n");

    char registry[REGISTRY_MAX];
    registry[0] = '\0';

    EmbedContext ctx = {.out = out, .registry = registry, .registry_len = 0};

    for (int i = 0; i < num_dirs; i++)
    {
        DirWalk(dirs[i], embed_callback, &ctx);
    }

    fprintf(out, "static const EmbeddedAsset g_embedded_assets[] = {\n");
    fprintf(out, "%s", registry);
    fprintf(out, "    {0, 0, 0}\n");
    fprintf(out, "};\n\n");
    fclose(out);
}