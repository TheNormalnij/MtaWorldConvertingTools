#pragma once

#include <filesystem>
#include <vector>
#include "../CIMG.h"

class CImgRepacker
{
public:
    CImgRepacker(std::filesystem::path path);
    ~CImgRepacker();

    bool Create();
    void Close();

    void AddImportedImg(CIMG *img) { m_import.push_back(img); };

    bool AddFile(const char* name, const std::vector<char> &buff);

    bool ExportFile(std::string &name);

private:
    const std::filesystem::path m_outputPath;
    std::vector<CIMG*> m_import;
    CIMG* m_currentOutput;
};
