#include "CMapDataWriter.h"

void CMapDataWriter::Write(std::vector<SIplInfo> &ipl)
{
    m_stream << "-- Generated with converter version " << PROJECT_VEWSION << "\n";
    m_stream << "return {\n";

    // Col map
    m_stream << "\tcolmap = {\n";

    m_stream << "\t};\n";

    // Id models from GTA SA
    m_stream << "\tstandart = {\n";

    m_stream << "\t};\n";

    // Defs
    m_stream << "\tdefs = {\n";

    m_stream << "\t};\n";

    // Map
    m_stream << "\tmap = {\n";
    // for (const SIplInfo &item : ipl) {
    //     m_stream << "\t\t{" << item.modelId << ", " << item.x << ", " << item.y << ", "
    //              << item.z << ", " << item.interior;
    // }
    m_stream << "\t};\n";

    // Water
    m_stream << "\twater = {\n";

    m_stream << "\t};\n";

    m_stream << "}\n";
}
