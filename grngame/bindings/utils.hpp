#pragma once

#include <filesystem>
#include <string>

std::filesystem::path BuildModulePath(const char *module_name, const char *extension);
bool ReadTextFile(const std::filesystem::path &path, std::string &out_content);