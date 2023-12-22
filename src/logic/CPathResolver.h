#pragma once

#include <filesystem>
#include "QStringView"

namespace fs = std::filesystem;

// TODO: add cache
class CPathResolver
{
public:
    CPathResolver() {};

    void Resolve(const fs::path &root, const std::string &add, fs::path &out);
private:
    bool ResolveRecursive(fs::path &root, QStringView add);

};
