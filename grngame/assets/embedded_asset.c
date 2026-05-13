#include "embedded_asset.h"
#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTRY_MAX 200000

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

    unsigned char buffer[16];
    long index = 0;

    while (index < size)
    {
        size_t read = fread(buffer, 1, 16, fs);

        fprintf(out, "    ");

        for (size_t i = 0; i < read; i++)
        {
            fprintf(out, "0x%02X", buffer[i]);
            index++;

            if (index < size)
                fprintf(out, ", ");
        }

        fprintf(out, "\n");
    }

    fprintf(out, "};\n#define %s_size %ld\n\n", var_name, size);

    fclose(fs);
}
static void embed_callback(const char *path, void *userdata)
{
    EmbedContext *ctx = (EmbedContext *)userdata;

    if (!FileIsLoadableScript(path) && !FileIsLoadableAudio(path) && !FileIsLoadableImage(path) &&
        !FileIsLoadableText(path))
        return;

    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;

    char var_name[256];
    snprintf(var_name, sizeof(var_name), "embedded_%s", base);

    for (char *c = var_name; *c; c++)
    {
        if (*c == '.' || *c == '-' || *c == ' ')
            *c = '_';
    }

    serialize_file(ctx->out, path, var_name);

    fprintf(ctx->out, "// asset: %s = %s\n\n", path, var_name);

    char entry[1024];
    snprintf(entry, sizeof(entry), "    {\"%s\", %s, %s_size},\n", base, var_name, var_name);
    if (!strstr(ctx->registry, entry))
    {
        strcat(ctx->registry, entry);
    }
}

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header)
{
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

    EmbedContext ctx = {.out = out, .registry = registry};

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