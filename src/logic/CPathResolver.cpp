#include "CPathResolver.h"

#include <QString>

bool CPathResolver::ResolveRecursive(fs::path &root, QStringView add) {
    auto to = add.indexOf('\\');

    QStringView searchStr;

    if (to == -1) {
        searchStr = add;
    } else {
        searchStr = QStringView(add.begin(), add.begin() + to);
    }

    for (const auto &dir : fs::directory_iterator(root)) {
        const auto& fileName = dir.path().filename();
        QString qPath = QString(fileName.c_str()).toLower();
        if (searchStr.compare(qPath) == 0) {
            if (to == -1) {
                root = root / fileName;
                return true;
            } else {
                root = root / fileName;
                return ResolveRecursive(root, QStringView(add.begin() + to + 1, add.end()));
            }
        }
    }

    return false;
}

void CPathResolver::Resolve(const fs::path &root, const std::string &add, fs::path &out)
{
    if (!fs::exists(root)) {
        return;
    }

    QString additionalPath = add.c_str();
    additionalPath = additionalPath.toLower();

    fs::path curr = root;
    if (ResolveRecursive(curr, additionalPath)) {
        out = curr;
    }
}
