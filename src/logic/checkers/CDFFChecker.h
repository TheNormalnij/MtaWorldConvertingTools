#pragma once

#include "CDFF.h"
#include <string>

class CDFFChecker
{
public:
    CDFFChecker(CDFF* dff): m_dff{dff} {};

    bool Check();
    const std::string_view GetError() const noexcept { return error; };
private:
    bool CheckEmpty();
    bool CheckVersion();
    bool CheckSize();
    bool CheckType();
private:
    CDFF* m_dff;
    std::string error;
};
