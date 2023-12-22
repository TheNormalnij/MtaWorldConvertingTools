#include "CGtaDatLoader.h"

#include <string>

void CGtaDatLoader::Read()
{
    std::string buff;

    while(!m_stream.eof()) {
        ReadLine(buff);
        if (buff.empty()) {
            break;
        }

        if (buff.ends_with('\r')) {
            buff.resize(buff.size() - 1);
        }

        if (buff.starts_with("IDE ")) {
            m_data.emplace_back(EDatType::IDE, &buff[4]);
        } else if (buff.starts_with("IPL ")) {
            m_data.emplace_back(EDatType::IPL, &buff[4]);
        } else if (buff.starts_with("IMG ")) {
            m_data.emplace_back(EDatType::IMG, &buff[4]);
        }
        buff.clear();
    }
}
