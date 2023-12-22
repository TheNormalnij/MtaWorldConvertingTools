#pragma once

#include "CLoader.h"
#include <vector>

enum class EDatType {
    IPL,
    IDE,
    IMG,
};

struct SGtaDatSection
{
    SGtaDatSection(EDatType _type, std::string _path): type(_type), realtivePath(std::move(_path)) {};

    EDatType type;
    std::string realtivePath;
};

class CGtaDatLoader : public CLoader
{
public:
    CGtaDatLoader(std::filesystem::path &&path): CLoader(std::move(path)) {};

    void Read();

    auto &GetFiles() noexcept { return m_data; };

private:
    std::vector<SGtaDatSection> m_data;
};
