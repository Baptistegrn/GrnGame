#include "grngame/bindings/utils.hpp"

#include <fstream>
#include <iterator>

#include "grngame/platform/directories.h"

std::filesystem::path BuildModulePath(const char *module_name, const char *extension)
{
    return std::filesystem::path(DirOfExecutable()) / "scripts" / (std::string(module_name) + extension);
}

bool ReadTextFile(const std::filesystem::path &path, std::string &out_content)
{
    std::ifstream input(path, std::ios::binary);
    if (!input.is_open())
        return false;

    out_content.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    return true;
}