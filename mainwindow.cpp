#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "CConverter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ConvertButton, SIGNAL(clicked()), this, SLOT(StartConverting()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartConverting()
{

}

void MainWindow::SelectGtaPath() {}

void MainWindow::SelectModPath() {}

void MainWindow::SelectOutputPath() {}
