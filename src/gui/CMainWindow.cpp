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
    CConverter converter(m_guiLogger, m_params);
    converter.Convert();
}

void CMainWindow::SelectPath(fs::path &to, QString info, QLabel *label)
{
    QString dir = QFileDialog::getExistingDirectory(this, info, to.c_str(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    to = dir.toStdString();
    label->setText(dir);
}

void CMainWindow::SelectGtaPath() {
    SelectPath(m_params.gtaPath, tr("Select GTA SA folder"), ui->gta_path_final);
}

void CMainWindow::SelectModPath() {
    SelectPath(m_params.modPath, tr("Select GTA mod folder"), ui->mod_path_final);
}

void CMainWindow::SelectOutputPath() {
    SelectPath(m_params.outputPath, tr("Select output folder"), ui->output_path_final);
}

void CMainWindow::ApplyConfig()
{
    ui->gta_path_final->setText(m_params.gtaPath.c_str());
    ui->mod_path_final->setText(m_params.modPath.c_str());
    ui->output_path_final->setText(m_params.outputPath.c_str());
}
