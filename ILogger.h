#pragma once

class ILogger
{
public:
    virtual void Verbose(const char*) = 0;
    virtual void Info(const char*) = 0;
    virtual void Warning(const char*) = 0;
    virtual void Error(const char*) = 0;
};
