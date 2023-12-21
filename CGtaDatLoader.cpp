#include "CGtaDatLoader.h"

#include <string>

void CGtaDatLoader::Read()
{
    std::string buff(201, '\000');
    while(m_stream.getline(buff.data(), 200)) {
        if (buff.starts_with("IDE ")) {
            m_data.emplace_back(EDatType::IDE, &buff[4]);
        } else if (buff.starts_with("IPL ")) {
            m_data.emplace_back(EDatType::IPL, &buff[4]);
        } else if (buff.starts_with("IMG ")) {
            m_data.emplace_back(EDatType::IMG, &buff[4]);
        }
    }
}
