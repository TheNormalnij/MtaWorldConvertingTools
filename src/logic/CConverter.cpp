#include "CConverter.h"

#include <unordered_set>
#include "writers/CMapDataWriter.h"
#include "writers/CMetaWriter.h"
#include "writers/CImgRepacker.h"
#include "writers/CRegisterWriter.h"
#include "checkers/CDFFChecker.h"

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
            m_log->Error("Can not continue without gta.dat");
            return;
        }

        LoadModWaterData();

        // Default files
        m_modFiles.emplace_back(SGtaDatSection(EDatType::IMG, "models\\gta3.img"));
        m_modFiles.emplace_back(SGtaDatSection(EDatType::IMG, "models\\gta_int.img"));

        if (!LoadModModelDefs()) {
            m_log->Error("Can not continue without all mod IDE's");
            return;
        }

        if (!LoadModIpls()) {
            m_log->Error("Can not continue without all mod IPL's");
            return;
        }

        if (m_settings.removeLods) {
            RemoveLods();
        } else {
            ReorderLods();
        }

        FilterUnusedModels();

        if (!OpenModIMGs()){
            m_log->Error("Can not continue without mod IMG's");
            return;
        }

        RemoveBrokenModels();

        if (!LoadModPhysicalInfo()) {
            m_log->Error("Can not continue without mod objects.dat");
            return;
        }

        ConvetMapInfoToMTA();

        GenerateColLib();

        WriteIMGs();

        CloseModIMGs();

        WriteMapInfo();

        if (m_settings.genMeta) {
            WriteMeta();
        }

        if (m_settings.genRegister) {
            WriteRegister();
        }

    } catch (...) {
        m_log->Error("Unknown error");
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

    m_log->Verbose("Readed %d lines from mod gta.dat", m_modFiles.size());

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
            m_log->Error( "Can not open mod IDE file: %s", &filePathData.realtivePath);
            return false;
        }

        ideLoader.Read(m_atomic, m_timed, m_clump);

        ideLoader.Close();
    }

    m_log->Verbose("Total loaded model defs: atomic %d, timed %d, clump %d", m_atomic.size(), m_timed.size(), m_clump.size());

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

        iplLoader.Read(m_modMap);

        iplLoader.Close();
    }

    return true;
}

bool CConverter::LoadModWaterData() {
    m_log->Info("Load mod water.dat");

    CWaterLoader waterLoader{m_settings.modPath / "data/water.dat"};

    if (waterLoader.Open()) {
        waterLoader.Read(m_waterinfo);

        waterLoader.Close();

        m_log->Info("Found %d water points", m_waterinfo.size());
        return true;
    } else {
        m_log->Warning("Can not open data/water.dat");
        return false;
    }
}

bool CConverter::LoadModPhysicalInfo()
{
    m_log->Info("Load mod object.dat");

    CObjectsDatLoader datLoader {m_settings.modPath / "data/object.dat"};

    if (!datLoader.Open()) {
        return false;
    }

    std::vector<SObjectsInfo> physInfo;
    datLoader.Read(physInfo);

    datLoader.Close();

    CPhysicalDataConvertor convertor(&physInfo, &m_usedModels);
    convertor.Convert(m_physical);

    return true;
}

void CConverter::RemoveBrokenModels()
{
    m_log->Info("Remove broken models");

    auto getImgFile = [&](const std::string &fileName, std::vector<char> &data) {
        for (CIMG &img : m_modImgs) {
            auto fileInfo = img.GetFileInfo(fileName);
            if (fileInfo) {
                img.UnpackFile(fileInfo, data);
                return true;
            }
        }
        m_log->Error("Can not find %s in IMGs", fileName);
        return false;
    };

    auto isValidDff = [&](const std::string &modelName) {
        std::vector<char> modelData;
        const bool unpackStatus = getImgFile(modelName + ".dff", modelData);

        if (!unpackStatus) {
            return false;
        }

        CDFF dff{std::move(modelData)};
        CDFFChecker checker{&dff};

        const bool checkStatus = checker.Check();
        if (!checkStatus) {
            m_log->Error("Broken DFF: %s. Reason: %s", modelName.c_str(), checker.GetError().data());
        }
        return checkStatus;
    };

    auto removeModelFromMap = [&](uint32_t modelId) {
        size_t deletedObjectsCount = 0;
        for (size_t i = 0; i < m_modMap.size(); i++) {
            if (m_modMap[i].modelId != modelId) {
                continue;
            }

            for (int32_t j = i + 1; j < m_modMap.size(); j++) {
                int32_t& lod = m_modMap[j].lod;
                if (lod == i) {
                    lod = -1;
                } else if (lod > i) {
                    lod--;
                }

                m_modMap[j - 1] = m_modMap[j];
            }

            deletedObjectsCount++;
        }

        if (deletedObjectsCount) {
            m_log->Warning("Removed %d map objects", deletedObjectsCount);
            m_modMap.resize(m_modMap.size() - deletedObjectsCount);
        }
    };

    auto filterBroken = [&](auto &container) {
        std::remove_reference_t<decltype(container)> temp{};
        for (const auto &def : container) {
            const std::string modelName = def.modelName.GetLowerString();
            if (isValidDff(modelName)) {
                temp.emplace_back(def);
            } else {
                removeModelFromMap(def.modelId);
            }
        }
        container.swap(temp);
    };

    filterBroken(m_atomic);
    filterBroken(m_timed);
    filterBroken(m_clump);
}

void CConverter::RemoveLods()
{
    m_log->Info("Remove LOD's");

    for (size_t i = 0; i < m_modMap.size(); i++) {
        const auto lod = m_modMap[i].lod;
        if (lod != -1) {
            m_modMap[lod].modelId = 0;
            m_modMap[i].lod = -1;
        }
    }

    std::vector<SIplInfo> filtered;
    filtered.reserve(m_modMap.size());

    for (int i = m_modMap.size() - 1; i >= 0; i--) {
        if (m_modMap[i].modelId != 0) {
            filtered.emplace_back(m_modMap[i]);
        }
    }

    m_modMap.swap(filtered);

    std::string info = "Removed lods count: " + std::to_string((filtered.size() - m_modMap.size()));
    m_log->Verbose((info.c_str()));
}

void CConverter::ReorderLods()
{
    m_log->Info("Reorder LOD's");
    std::vector<SIplInfo> filtered;

    for (int i = 0; i < m_modMap.size(); i++) {
        const int32_t lodPos = m_modMap[i].lod;
        if (lodPos > i) {
            m_modMap[i].lod = i;
            const SIplInfo lod = m_modMap[lodPos];
            m_modMap[lodPos] = m_modMap[i];
            m_modMap[i] = lod;
        }
    }
}

void CConverter::FilterUnusedModels()
{
    m_log->Info("Filter unused model def's");

    std::unordered_set<uint32_t> usedSet;
    for (const auto &posInfo : m_modMap) {
        usedSet.insert(posInfo.modelId);
    }

    auto filter = [&](auto &container) {
        std::remove_reference_t<decltype(container)> temp{};
        for (const auto &def : container) {
            if (usedSet.contains(def.modelId)) {
                temp.emplace_back(def);
                m_usedModels.insert(std::move(def.modelName.GetLowerString()));
            }
        }
        container.swap(temp);
    };

    filter(m_atomic);
    filter(m_timed);
    filter(m_clump);

    m_log->Verbose("Used models count: %d", usedSet.size());
}

bool CConverter::OpenModIMGs()
{
    m_log->Info("Open mod IMG's");

    for (const auto &filePathData : m_modFiles) {
        if (filePathData.type != EDatType::IMG) {
            continue;
        }

        fs::path path;
        MakePath(m_settings.modPath, filePathData.realtivePath, path);

        CIMG &img = m_modImgs.emplace_back(path);

        if (!img.Open()) {
            m_log->Error("Can not open mod IMG file: %s", filePathData.realtivePath.c_str());
            return false;
        }
    }

    return true;
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

    for (CIMG &img : m_modImgs) {
        for (const SImgFileInfo &fileInfo : img.GetFilesInfo()) {
            auto name = fileInfo.szFileName.GetLowerString();
            // TODO end with
            if (name.find(".col") == name.npos) {
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

                    std::string modelName = out.GetName().GetLowerString();
                    if (m_usedModels.contains(modelName)) {
                        const size_t from = m_cols.GetSize();
                        m_cols.Add(out);
                        m_colMap[modelName] = {from + 1, m_cols.GetSize()};
                    }
                } else {
                    break;
                }

            }

        }
    }

    m_log->Verbose("Total col's count: %d", m_colMap.size());
}

void CConverter::GetUsedTxd(std::unordered_set<std::string> &out)
{
    auto filter = [&](auto &container) {
        for (const auto &def : container) {
            out.insert(std::move(def.texDictName.GetLowerString()));
        }
    };

    filter(m_atomic);
    filter(m_timed);
    filter(m_clump);
}

void CConverter::ConvetMapInfoToMTA() {
    m_log->Info("Convert map to MTA format");
    const size_t count = m_modMap.size();

    std::unordered_map<uint32_t, uint32_t> modelDefId{};
    std::unordered_map<std::string, uint32_t> modelNameToDef{};

    size_t offset = 0;
    auto fill = [&](auto &container) {
        for (uint32_t i = 0; i < container.size(); i++) {
            const uint32_t defId = i + offset;
            modelDefId[container[i].modelId] = defId;
            modelNameToDef[container[i].modelName.GetLowerString()] = defId;
        }
        offset += container.size();
    };

    fill(m_atomic);
    fill(m_timed);
    fill(m_clump);

    m_mtaMap.resize(count);
    for (int i = 0; i < count; i++) {
        convertPosForMta(m_modMap[i], m_mtaMap[i]);
        m_mtaMap[i].modelDef = modelDefId[m_modMap[i].modelId] + 1;
        const int32_t lod = m_modMap[i].lod;
        if (lod >= 0) {
            m_mtaMap[lod].isLod = true;
        }
    }
    m_modMap.clear();

    for (const auto &group : m_physical.models) {
        std::vector<uint32_t> groupInfo{};
        groupInfo.reserve(group.size());
        for (const std::string &modelName : group) {
            const auto id = modelNameToDef.find(modelName);
            if (id != modelNameToDef.end()) {
                groupInfo.push_back(id->second);
            }
        }
        m_physical.defs.push_back(groupInfo);
    }

    // Remove unsupported flags
    auto fixFlags = [&](auto &group) {
        for (auto &info : group) {
            // Disable garage door flag
            // It causes crash
            info.flags &= 0xFFFFF7FF;
        }
    };

    fixFlags(m_atomic);
    fixFlags(m_timed);
    fixFlags(m_clump);
}

void CConverter::WriteIMGs()
{
    m_log->Info("Starting repack IMGs");

    CImgRepacker imgRepacker(m_settings.outputPath);

    if (!imgRepacker.Create()) {
        m_log->Error("Can not create IMG output files");
        return;
    }

    for (CIMG &img : m_modImgs) {
        imgRepacker.AddImportedImg(&img);
    }

    // Pack col library
    if (m_cols.GetSize() > 0) {
        if (!imgRepacker.AddFile("allmapcolls.col", m_cols.GetData()) ) {
            m_log->Error("Can not write allmapcolls.col in IMG");
        }
    } else {
        m_log->Warning("Skip empty allmapcolls.col");
    }

    std::unordered_set<std::string> usedTxds{};
    GetUsedTxd(usedTxds);

    // Pack txd's
    for (std::string name : usedTxds) {
        name.shrink_to_fit();
        name.append(".txd");
        if (!imgRepacker.ExportFile(name)) {
            m_log->Error("Error writing: %s", name.c_str());
        }
    }

    // Pack dff's
    for (std::string name : m_usedModels) {
        name.shrink_to_fit();
        name.append(".dff");
        if (!imgRepacker.ExportFile(name)) {
            m_log->Error("Error writing: %s", name.c_str());
        }
    }


    imgRepacker.Close();
}

void CConverter::WriteMapInfo()
{
    m_log->Info("Write world.lua");

    CMapDataWriter mapWriter(std::move(m_settings.outputPath / "world.lua"));

    mapWriter.Create();

    CIMG outputImg = CIMG(m_settings.outputPath / "world1.img");
    outputImg.Open();

    mapWriter.SetIMG(&outputImg);
    mapWriter.SetColMap(&m_colMap);
    mapWriter.SetIplInfo(&m_mtaMap);

    mapWriter.SetAtomic(&m_atomic);
    mapWriter.SetTimed(&m_timed);
    mapWriter.SetClump(&m_clump);

    mapWriter.SetWater(&m_waterinfo);
    mapWriter.SetPhysicalInfo(&m_physical);

    mapWriter.Write();

    mapWriter.Close();

    outputImg.Close();
}

void CConverter::WriteMeta()
{
    m_log->Info("Write meta.xml");

    CMetaWriter metaWriter( m_settings.outputPath / "meta.xml" );

    if (!metaWriter.Create()) {
        m_log->Error("Can not create meta.xml file");
        return;
    }

    metaWriter.AddFile("world1.img");
    metaWriter.AddFile("world.lua");

    metaWriter.Write();

    metaWriter.Close();
}

void CConverter::WriteRegister()
{
    m_log->Info("Write register.lua");

    CRegisterWriter registerWriter(m_settings.outputPath / "register.lua", m_settings.worldName);

    if (!registerWriter.Create()) {
        m_log->Error("Can not create register.xml file");
        return;
    }

    registerWriter.Write();

    registerWriter.Close();
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
