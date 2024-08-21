
#include "CRegisterWriter.h"

void CRegisterWriter::Write()
{
    Printf("exports['Streamer']:registerWorld{\n"
           "\tname = '%s';\n"
           "\timgs = { 'world1.img'; };\n"
           "\tmapPath = 'world.lua';\n"
           "\tmapType = 'lua';\n"
           "}\n",
           m_worldName.c_str());
}
