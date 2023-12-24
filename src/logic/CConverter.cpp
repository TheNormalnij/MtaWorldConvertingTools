#include "CConverter.h"

#include <unordered_set>
#include "CMapDataWriter.h"
#include "CMetaWriter.h"
#include "CImgRepacker.h"

#ifndef _WIN32
    #include "CPathResolver.h"
#endif

namespace fs = std::filesystem;

CConverter::CConverter(ILogger *logger, SConverterParams *settings): m_log(logger), m_settings(*settings) {
}

void CConverter::Convert()
{
    m_log->Info("Start converting...");

    try {
        // This fixes float values reading in sscanf
        setlocale(LC_NUMERIC, "C");

        if (!LoadModGtaDat()) {
            m_log->Error("Stopped");
            return;
        }

        if (!LoadModModelDefs()) {
            m_log->Error("Stopped");
            return;
        }

        if (!LoadModIpls()) {
            m_log->Error("Stopped");
            return;
        }

        if (m_settings.removeLods) {
            RemoveLods();
        }

        FilterUnusedModels();

        GenerateColLib();

        OpenModIMGs();

        WriteIMGs();

        CloseModIMGs();

        WriteMapInfo();

        if (m_settings.genMeta) {
            WriteMeta();
        }
    } catch (...) {
        m_log->Error("Runtime error");
    }

    m_log->Info("Finished");
}

bool CConverter::LoadModGtaDat()
{
    m_log->Info("Load mod gta.dat");

    fs::path path = m_settings.modPath / "data/gta.dat";
    CGtaDatLoader datLoader(std::move(path));

    if (!datLoader.Open()) {
        m_log->Error("Can not read gta.dat from mod");
        return false;
    }

    datLoader.Read();
    datLoader.Close();

    m_modFiles.swap(datLoader.GetFiles());

    std::string msg = "Readed " + std::to_string(m_modFiles.size()) + " lines from mod gta.dat";
    m_log->Verbose(msg.c_str());

    return true;
}

bool CConverter::LoadModModelDefs()
{
    m_log->Info("Load mod IDE's");

    for (const auto &filePathData : m_modFiles) {
        if (filePathData.type != EDatType::IDE) {
            continue;
        }

        fs::path fullPath;
        MakePath(m_settings.modPath, filePathData.realtivePath, fullPath);

        CIdeLoader ideLoader(std::move(fullPath));

        if (!ideLoader.Open()) {
            std::string err = "Can not open mod IDE file: " + filePathData.realtivePath;
            m_log->Error(err.c_str());
            return false;
        }

        ideLoader.Read(m_atomic, m_timed, m_clump);

        ideLoader.Close();
    }

    return true;
}

bool CConverter::LoadModIpls()
{
    m_log->Info("Load mod IPL's");

    for (const auto &filePathData : m_modFiles) {
        if (filePathData.type != EDatType::IPL) {
            continue;
        }

        fs::path fullPath;
        MakePath(m_settings.modPath, filePathData.realtivePath, fullPath);

        CIplLoader iplLoader(std::move(fullPath));

        if (!iplLoader.Open()) {
            std::string err = "Can not open mod IPL file: " + filePathData.realtivePath;
            m_log->Error(err.c_str());
            return false;
        }

        // TODO lod linking
        iplLoader.Read(m_modMap);

        iplLoader.Close();
    }

    return true;
}

void CConverter::RemoveLods()
{
    m_log->Info("Remove LOD's");
    std::vector<SIplInfo> filtered;

    for (int i = m_modMap.size(); i >= 0; i--) {
        const auto lod = m_modMap[i].lod;
        if (lod != -1) {
            m_modMap[lod].modelId = 0;
        }

        if (m_modMap[i].modelId != 0) {
            filtered.emplace_back(m_modMap[i]);
        }
    }

    m_modMap.swap(filtered);
}

void CConverter::FilterUnusedModels()
{
    m_log->Info("Load mod IDE's");

    std::unordered_set<uint32_t> usedSet;
    for (const auto &posInfo : m_modMap) {
        usedSet.insert(posInfo.modelId);
    }

    auto filter = [&](auto &container) {
        typeof(container) temp;
        for (const auto &def : container) {
            if (usedSet.contains(def.modelId)) {
                temp.emplace_back(def);
                m_usedModels.insert(std::string(&def.modelName[0], &def.modelName[20]));
            }
        }
        container.swap(temp);
    };

    filter(m_atomic);
    filter(m_timed);
    filter(m_clump);
}

void CConverter::OpenModIMGs()
{
    m_log->Info("Open mod IMG's");

    fs::path path;
    MakePath(m_settings.gtaPath, "models\\gta3.img", path);

    CIMG &img = m_modImgs.emplace_back(path);
    if (!img.Open()) {
        m_log->Error("Can not open mod gta3.img");
        return;
    }

    for (const auto &filePathData : m_modFiles) {
        if (filePathData.type != EDatType::IMG) {
            continue;
        }

        path.clear();
        MakePath(m_settings.modPath, filePathData.realtivePath, path);

        CIMG &img = m_modImgs.emplace_back(path);

        if (!img.Open()) {
            std::string err = "Can not open mod IMG file: " + filePathData.realtivePath;
            m_log->Error(err.c_str());
            return;
        }
    }
}

void CConverter::CloseModIMGs()
{
    m_log->Info("Close mod IMG's");
    for (CIMG &img : m_modImgs) {
        img.Close();
    }

    m_modImgs.clear();
}

void CConverter::GenerateColLib()
{
    m_log->Info("Generate col lib");

    std::unordered_set<std::string> usedModels;

    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> colMap;

    for (CIMG &img : m_modImgs) {
        for (const SImgFileInfo &fileInfo : img.GetFilesInfo()) {
            auto name = std::string_view(fileInfo.szFileName, 24);
            // TODO end with
            if (name.find(".col") != name.npos) {
                continue;
            }

            std::vector<char> buff;
            img.UnpackFile(&fileInfo, buff);

            CColLib imgCol(std::move(buff));

            size_t offset = 0;

            // Can be optimized here
            while (true) {
                CColLib out;

                if (imgCol.Unpack(out, offset)) {
                    offset += out.GetSize();

                    std::string modelName(out.GetName());
                    if (usedModels.contains(modelName)) {
                        const size_t from = m_cols.GetSize();
                        m_cols.Add(out);
                        colMap[modelName] = {from, offset};
                    }
                };

                break;
            }

        }
    }
}

void CConverter::GetUsedTxd(std::unordered_set<std::string> &out)
{
    auto filter = [&](auto &container) {
        for (const auto &def : container) {
            out.insert(def.texDictName);
        }
    };

    filter(m_atomic);
    filter(m_timed);
    filter(m_clump);
}

void CConverter::WriteIMGs()
{
    m_log->Info("Starting repack IMGs");

    CImgRepacker imgRepacker(m_settings.outputPath);

    for (CIMG &img : m_modImgs) {
        imgRepacker.AddImportedImg(&img);
    }

    // Pack col library
    if (!imgRepacker.AddFile("allmapcolls.col", m_cols.GetData()) ) {
        m_log->Error("Can not write allmapcolls.col in IMG");
    }

    std::unordered_set<std::string> usedTxds{};
    GetUsedTxd(usedTxds);

    // Pack txd's
    for (std::string name : usedTxds) {
        name.shrink_to_fit();
        name.append(".txd");
        if (!imgRepacker.ExportFile(name.c_str())) {
            m_log->Error(std::string("Error writing: " + name).c_str());
        }
    }

    // Pack dff's
    for (std::string name : m_usedModels) {
        name.shrink_to_fit();
        name.append(".dff");
        if (!imgRepacker.ExportFile(name.c_str())) {
            m_log->Error(std::string("Error writing: " + name).c_str());
        }
    }
}

void CConverter::WriteMapInfo()
{
    CMapDataWriter mapWriter(std::move(m_settings.outputPath / "register.lua"));

    mapWriter.Create();

    mapWriter.Close();
}

void CConverter::WriteMeta()
{
    m_log->Info("Write meta.xml");

    CMetaWriter metaWriter( std::move(m_settings.outputPath / "meta.xml") );

    if (!metaWriter.Create()) {
        m_log->Error("Can not create meta.xml file");
        return;
    }

    metaWriter.AddFile("world1.img");
    metaWriter.AddFile("world.lua");

    metaWriter.Write();

    metaWriter.Close();
}

void CConverter::MakePath(const fs::path &root, const std::string &add, fs::path &out)
{
#ifndef _WIN32
    static CPathResolver resolver{};
    resolver.Resolve(root, add, out);
#else
    out = root/add;
#endif
}
