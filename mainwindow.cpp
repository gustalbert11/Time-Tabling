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

    ui->tableProfessors->setColumnCount(6);
    ui->tableProfessors->setHorizontalHeaderLabels(
        {"ID", "Secciones", "Max horas diario", "Max horas consecutivo","Tipo de Pref","Descripcion Pref"}
    );
    ui->tableProfessors->setColumnWidth(0, 150);
    ui->tableProfessors->setColumnWidth(1, 150);
    ui->tableProfessors->setColumnWidth(2, 150);
    ui->tableProfessors->setColumnWidth(3, 150);
    ui->tableProfessors->setColumnWidth(4, 200);
    ui->tableProfessors->setColumnWidth(5, 200);

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

    dm_instance.clear_all_data();

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

    bool ok = dm_instance.import_from_JSON(filename.toStdString());

    
    if (ok) {
        QMessageBox::information(this, "Éxito", "El archivo JSON fue importado correctamente.");
        mostrar_profesores_en_tabla();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo procesar el archivo JSON.");
    }
}

void MainWindow::mostrar_profesores_en_tabla()
{
    const auto& professors = dm_instance.get_professors();

    ui->tableProfessors->setRowCount(static_cast<int>(professors.size()));

    int row = 0;
    for (const auto &pair : professors)
    {
        const auto &p = pair.second.get();

        ui->tableProfessors->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(p->get_id())));
        ui->tableProfessors->setItem(row, 1, new QTableWidgetItem(QString::number(p->get_num_sections())));
        ui->tableProfessors->setItem(row, 2, new QTableWidgetItem(QString::number(p->get_max_daily_hours())));
        ui->tableProfessors->setItem(row, 3, new QTableWidgetItem(QString::number(p->get_max_consecutive_hours())));
        ui->tableProfessors->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(preference_type_to_string(p->get_preference()->get_type()))));
        ui->tableProfessors->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(p->get_preference()->get_description())));
        row++;
    }
}