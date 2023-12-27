
#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

struct SImgFileInfo
{
    unsigned int   uiOffset;
    unsigned short usSize;
    unsigned short usUnpackedSize;
    char           szFileName[24];
};

class CIMG
{
public:
    CIMG(fs::path path): m_path(std::move(path)){};

    bool Create();
    bool Open();
    bool Close();

    bool UnpackFile(size_t pos, std::vector<char> &buff);
    bool UnpackFile(const SImgFileInfo *info, std::vector<char> &buff);

    bool AddFile(std::string_view fileName, const char *content, size_t count);

    const char* GetContentFileName(size_t pos) const noexcept { return m_filesInfo[pos].szFileName; };
    size_t GetFilesCount() const noexcept { return m_filesInfo.size(); };

    // Why can't i use const here?
    const SImgFileInfo *GetFileInfo(const char* name) { return m_fileMap[name]; };

    size_t GetSize();

    auto &GetFilesInfo() const noexcept { return m_filesInfo; };
private:
    void PrepareFileMap();
    bool HasHeaderSizeForNextElement();
    void ExtentHeader();

    void UpdateHeader();

private:
    std::fstream m_stream;
    fs::path m_path;
    bool m_readOnly;

    size_t m_startDataOffset;

    std::vector<SImgFileInfo> m_filesInfo;
    std::unordered_map<std::string, SImgFileInfo*> m_fileMap;
};

