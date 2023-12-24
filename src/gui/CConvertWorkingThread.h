#pragma once

#include <QThread>

#include "CLoggerThreadProxy.h"
#include "../logic/CConverter.h"

class CConvertWorkingThread : public QThread {
public:
    CConvertWorkingThread(ILogger *logger, SConverterParams &settings): m_logger(logger), m_settings(settings) {};

    void run() {
        CConverter converter(&m_logger, &m_settings);
        converter.Convert();
    }

private:
    CLoggerThreadProxy m_logger;
    SConverterParams m_settings;
};
