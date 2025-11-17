#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::avanzar_ventana()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::volver_ventana()
{
    ui->stackedWidget->setCurrentIndex(0);
}


