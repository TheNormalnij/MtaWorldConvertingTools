
#pragma once

#include <QMainWindow>
#include <QLabel>

#include "CGuiTextBrowsertLogger.h"
#include "../logic/CConverter.h"
#include "CConvertWorkingThread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();


private slots:
    void StartConverting();
    void OnConvertingFinished();
    void SelectGtaPath();
    void SelectModPath();
    void SelectOutputPath();

private:
    void SelectPath(fs::path &to, QString info, QLabel *label);
    void ApplyConfig();

private:
    Ui::MainWindow *ui;
    SConverterParams m_settings;
    CGuiTextBrowsertLogger *m_guiLogger;

    CConvertWorkingThread *m_workThread;
};
