#include "CMapDataWriter.h"

void CMapDataWriter::Write()
{
    m_stream << "-- Generated with converter version " << PROJECT_VERSION << "\n";
    m_stream << "return {\n";

    // Col map
    m_stream << "\tcolmap = {\n";
    std::unordered_map<std::string, size_t> colPos;

    for (size_t i = 1; const auto &map : *m_pColMap) {
        colPos[map.first] = i;
        m_stream << "\t\t{ " << map.second.first << ", " << map.second.second << " };" << std::endl;
        i++;
    }

    m_stream << "\t};\n";

    // Id models from GTA SA
    m_stream << "\tstandart = {\n";

    m_stream << "\t};\n";

    // Defs
    m_stream << "\tdefs = {\n";

    m_stream << "\t};\n";

    // Map
    m_stream << "\tmap = {\n";

    m_stream << "\t};\n";

    // Water
    m_stream << "\twater = {\n";

    m_stream << "\t};\n";

    m_stream << "}\n";
}
