#include "CMainWindow.h"
#include "../../resources/ui_mainwindow.h"

#include <QFileDialog>

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->BtnConvert, SIGNAL(clicked()), this, SLOT(StartConverting()));

    //connect(ui->BtnSelectGtaPath, SIGNAL(clicked()), this, SLOT(SelectGtaPath()));
    connect(ui->BtnSelectModPath, SIGNAL(clicked()), this, SLOT(SelectModPath()));
    connect(ui->BtnSelectOutPath, SIGNAL(clicked()), this, SLOT(SelectOutputPath()));

    m_workThread = nullptr;
    m_guiLogger = new CGuiTextBrowsertLogger(ui->Log);

    m_settings.modPath = "/mnt/win/Games/GTA United 1.2";
    m_settings.removeLods = false;

    ApplyConfig();

    m_guiLogger->Info("Started");
}

CMainWindow::~CMainWindow()
{
    delete ui;
    delete m_guiLogger;
    if (m_workThread) {
        delete m_workThread;
    }
}

void CMainWindow::StartConverting()
{
    if (m_workThread != nullptr) {
        if (m_workThread->isRunning()) {
            m_guiLogger->Warning("Can not start");
            return;
        }

        delete m_workThread;
    }

    ui->Log->clear();

    m_settings.removeLods = ui->ChkRemoveLods->checkState() == Qt::Checked;

    m_workThread = new CConvertWorkingThread(m_guiLogger, m_settings);

    connect(m_workThread, SIGNAL(finished()), this, SLOT(OnConvertingFinished()));

    m_workThread->start();
}

void CMainWindow::OnConvertingFinished()
{
    m_guiLogger->Info("Converting thread finished");
    delete m_workThread;
    m_workThread = nullptr;
}

void CMainWindow::SelectPath(fs::path &to, QString info, QLabel *label)
{
    auto tostr = to.generic_u8string();
    QString dir = QFileDialog::getExistingDirectory(this, info, tostr.c_str(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    to = dir.toStdString();
    label->setText(dir);
}

void CMainWindow::SelectGtaPath() {
    //SelectPath(m_settings.gtaPath, tr("Select GTA SA folder"), ui->gta_path_final);
}

void CMainWindow::SelectModPath() {
    SelectPath(m_settings.modPath, tr("Select GTA mod folder"), ui->mod_path_final);
}

void CMainWindow::SelectOutputPath() {
    SelectPath(m_settings.outputPath, tr("Select output folder"), ui->output_path_final);
}

void CMainWindow::ApplyConfig()
{
    //ui->gta_path_final->setText(m_settings.gtaPath.generic_u8string().c_str());
    ui->mod_path_final->setText(m_settings.modPath.generic_u8string().c_str());
    ui->output_path_final->setText(m_settings.outputPath.generic_u8string().c_str());

    ui->ChkRemoveLods->setCheckState(m_settings.removeLods ? Qt::Checked : Qt::Unchecked);
}
