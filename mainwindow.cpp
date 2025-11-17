#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "../include/data_manager.hpp" 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Asegurarnos de empezar siempre en la página 0 (el menú principal)
    ui->stackedWidget->setCurrentIndex(0);

    // CONEXIÓN MANUAL
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::avanzar_ventana);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::volver_ventana);

    connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::importar_json);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::avanzar_ventana()
{
    ui->stackedWidget->setCurrentIndex(1);

    instance.clear_all_data();

}

void MainWindow::volver_ventana()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::importar_json()
{
    
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo JSON",
        "",
        "JSON Files (*.json)"
    );

    if (filename.isEmpty()) {
        return;
    }

    bool ok = instance.import_from_JSON(filename.toStdString());

    
    if (ok) {
        QMessageBox::information(this, "Éxito", "El archivo JSON fue importado correctamente.");
    } else {
        QMessageBox::warning(this, "Error", "No se pudo procesar el archivo JSON.");
    }
}
