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

    connect(ui->avanzarPg3Button, &QPushButton::clicked, this, &MainWindow::avanzar_pg2);

    connect(ui->formProfButton, &QPushButton::clicked, this, &MainWindow::show_professor_form);




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

void MainWindow::avanzar_pg2()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::show_professor_form()
{
    if (!professorWindow) {
        professorWindow = new SecondWindow(nullptr , &p);
        professorWindow->setAttribute(Qt::WA_DeleteOnClose);

        // Conectar la señal de destrucción
        connect(professorWindow, &SecondWindow::destroyed, this, &MainWindow::on_professor_window_closed);

        professorWindow->show();
    }
    else {
        professorWindow->raise();
        professorWindow->activateWindow();
    }
}

void MainWindow::on_professor_window_closed()
{
    professorWindow = nullptr;
}

