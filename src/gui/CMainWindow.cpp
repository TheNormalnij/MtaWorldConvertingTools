#include "CMainWindow.h"
#include "../../resources/ui_mainwindow.h"

#include <QFileDialog>

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->BtnConvert, SIGNAL(clicked()), this, SLOT(StartConverting()));

    connect(ui->BtnSelectGtaPath, SIGNAL(clicked()), this, SLOT(SelectGtaPath()));
    connect(ui->BtnSelectModPath, SIGNAL(clicked()), this, SLOT(SelectModPath()));
    connect(ui->BtnSelectOutPath, SIGNAL(clicked()), this, SLOT(SelectOutputPath()));


    m_guiLogger = new CGuiTextBrowsertLogger(ui->Log);


    ApplyConfig();

    m_guiLogger->Info("Started");
}

CMainWindow::~CMainWindow()
{
    delete ui;
    delete m_guiLogger;
}

void CMainWindow::StartConverting()
{
    CConverter converter(m_guiLogger, m_settings);
    converter.Convert();
}

void CMainWindow::SelectPath(fs::path &to, QString info, QLabel *label)
{
    QString dir = QFileDialog::getExistingDirectory(this, info, to.c_str(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    to = dir.toStdString();
    label->setText(dir);
}

void CMainWindow::SelectGtaPath() {
    SelectPath(m_settings.gtaPath, tr("Select GTA SA folder"), ui->gta_path_final);
}

void CMainWindow::SelectModPath() {
    SelectPath(m_settings.modPath, tr("Select GTA mod folder"), ui->mod_path_final);
}

void CMainWindow::SelectOutputPath() {
    SelectPath(m_settings.outputPath, tr("Select output folder"), ui->output_path_final);
}

void CMainWindow::ApplyConfig()
{
    ui->gta_path_final->setText(m_settings.gtaPath.c_str());
    ui->mod_path_final->setText(m_settings.modPath.c_str());
    ui->output_path_final->setText(m_settings.outputPath.c_str());
}
