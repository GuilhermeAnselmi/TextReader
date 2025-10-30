#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <string>

QString _filePath;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _filePath = nullptr;

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::OnMenuActionOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::OnMenuActionSave);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnMenuActionOpen() {
    _filePath = QFileDialog::getOpenFileName(this, tr("Select File Text (*.txt)"), QDir::homePath(), tr("*.txt"));

    std::ifstream file(_filePath.toStdString());

    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        QString content = QString::fromStdString(buffer.str());

        ui->plainTextEdit->setPlainText(content);

        file.close();
    }else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open file"));
    }
}

void MainWindow::OnMenuActionSave() {
    if (_filePath == nullptr) {
        QMessageBox::information(this, tr("File Not Open"), tr("File not open"));

        return;
    }

    QMessageBox::StandardButton question = QMessageBox::question(this, "Save", "Save and replace file?");

    if (question != QMessageBox::Yes) {
        return;
    }

    std::ofstream file(_filePath.toStdString());

    if (file.is_open()) {
        std::string content = ui->plainTextEdit->toPlainText().toStdString();

        file << content;

        file.close();

        QMessageBox::information(this, "Success", tr( "File successfully saved"));
    }else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to save file"));
    }
}
