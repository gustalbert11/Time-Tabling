#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   
    ui->setupUi(this);
    ui->insertCourseButton->hide();
    ui->ShowInfoButton->setText("Mostrar Materias");
    ui->tableInfo->setColumnCount(7);
    ui->tableInfo->setHorizontalHeaderLabels(
        {"ID","Nombre", "Secciones", "Max horas diario", "Max horas consecutivo","Tipo de Pref","Descripcion Pref"}
    );
    ui->tableInfo->setColumnWidth(0, 150);
    ui->tableInfo->setColumnWidth(1, 150);
    ui->tableInfo->setColumnWidth(2, 150);
    ui->tableInfo->setColumnWidth(3, 150);
    ui->tableInfo->setColumnWidth(4, 150);
    ui->tableInfo->setColumnWidth(5, 200);
    ui->tableInfo->setColumnWidth(6, 200);

    ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Asegurarnos de empezar siempre en la página 0 (el menú principal)
    ui->stackedWidget->setCurrentIndex(0);

    // CONEXIÓN MANUAL
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::avanzar_ventana);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::volver_ventana);

    connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::import_json);

    connect(ui->insertProfButton, &QPushButton::clicked, this, &MainWindow::open_prof_form);

    connect(ui->ShowInfoButton, &QPushButton::clicked, this, &MainWindow::update_table);

    connect(ui->insertCourseButton, &QPushButton::clicked, this, &MainWindow::open_course_form);

    connect(ui->tableInfo, &QTableWidget::itemClicked, this, &MainWindow::onItemClicked);

    connect(ui->insertSecButton, &QPushButton::clicked, this, &MainWindow::open_section_form);

    connect(ui->scheduleButton, &QPushButton::clicked, this, &MainWindow::create_schedule);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_schedule()
{
    fn_instance.init();
    if (fn_instance.solve_min_cost_max_flow())
    {
        auto sch = fn_instance.get_final_schedule();

        for(auto sc: sch)
        {
            std::cout << sc.professor_name<<"\n";
            std::cout << sc.course_name<<"\n";
            std::cout << sc.day<<"\n";
            std::cout << sc.start_hour<<"\n";
            std::cout << sc.end_hour<<"\n";
            std::cout << sc.cost<<"\n";
        }
    }
}

void MainWindow::avanzar_ventana()
{
    int it = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(it + 1);
    dm_instance.clear_all_data();

}

void MainWindow::volver_ventana()
{
    int it = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(it - 1);
}

void MainWindow::import_json()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo JSON",
        "",
        "JSON Files (*.json)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.import_from_json(filename.toStdString());

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "El archivo JSON fue importado correctamente.");
        showing_professors = !showing_professors;
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo procesar el archivo JSON.");
    }
}

void MainWindow::show_professors()
{
    const auto& professors = dm_instance.get_professors();

    ui->tableInfo->setRowCount(static_cast<int>(professors.size()));

    int row = 0;
    for (const auto &pair : professors)
    {
        const auto &p = pair.second.get();

        ui->tableInfo->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(p->get_id())));
        ui->tableInfo->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->get_name())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(p->get_num_sections())));
        ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(p->get_max_daily_hours())));
        ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::number(p->get_max_consecutive_hours())));
        ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(preference_type_to_string(p->get_preference()->get_type()))));
        ui->tableInfo->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(p->get_preference()->get_description())));
        row++;
    }
}

void MainWindow::open_prof_form()
{
    if (!prof_form) 
    {
        prof_form = new ProfessorForm();
        prof_form->setAttribute(Qt::WA_DeleteOnClose);

        // Conectar la señal de destrucción
        connect(prof_form, &ProfessorForm::destroyed, this, &MainWindow::on_professor_window_closed);

        prof_form->show();
    }
    else 
    {
        prof_form->raise();
        prof_form->activateWindow();
    }
}

void MainWindow::on_professor_window_closed()
{
    prof_form = nullptr;
    showing_professors = !showing_professors;
    update_table();
    
}

void MainWindow::show_courses()
{
    const auto& courses = dm_instance.get_courses();

    ui->tableInfo->setRowCount(static_cast<int>(courses.size()));       

    int row = 0;
    for (const auto &pair : courses)
    {
        const auto &p = pair.second.get();

        ui->tableInfo->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(p->get_id())));
        ui->tableInfo->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->get_name())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(p->get_level())));
        ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(p->get_num_credits())));
        ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::number(p->get_num_sections())));
        ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::number(p->get_num_weekly_hours())));
        ui->tableInfo->setItem(row, 6, new QTableWidgetItem(QString::number(p->get_max_daily_hours())));
        row++;
    }
}

void MainWindow::update_table()
{
    ui->tableInfo->clear();
    ui->tableInfo->setRowCount(0);
    ui->tableInfo->setColumnCount(0);
  
    if(showing_professors)
    {
        ui->insertProfButton->hide();
        ui->insertCourseButton->show();
        ui->ShowInfoButton->setText("Mostrar Profesores");
        ui->tableInfo->setColumnCount(6);
        ui->tableInfo->setHorizontalHeaderLabels(
        {"ID","Nombre", "Semestre", "U.C", "Secciones","Horas Semanales","Max Horas Diarias"}
        );
        ui->tableInfo->setColumnWidth(0, 180);
        ui->tableInfo->setColumnWidth(1, 180);
        ui->tableInfo->setColumnWidth(2, 180);
        ui->tableInfo->setColumnWidth(3, 180);
        ui->tableInfo->setColumnWidth(4, 180);
        ui->tableInfo->setColumnWidth(5, 180);
        ui->tableInfo->setColumnWidth(6, 180);

        show_courses();
        

        showing_professors = false;
    
    } 
    else
    {
        ui->insertProfButton->show();
        ui->insertCourseButton->hide();
        ui->ShowInfoButton->setText("Mostrar Materias");
        ui->tableInfo->setColumnCount(7);
        ui->tableInfo->setHorizontalHeaderLabels(
        {"ID","Nombre", "Secciones", "Max horas diario", "Max horas consecutivo","Tipo de Pref","Descripcion Pref"}
        );
        ui->tableInfo->setColumnWidth(0, 150);
        ui->tableInfo->setColumnWidth(1, 150);
        ui->tableInfo->setColumnWidth(2, 150);
        ui->tableInfo->setColumnWidth(3, 150);
        ui->tableInfo->setColumnWidth(4, 150);
        ui->tableInfo->setColumnWidth(5, 200);
        ui->tableInfo->setColumnWidth(6, 200);
        show_professors();
        
        showing_professors = true;
    }
}

void MainWindow::open_course_form()
{
    if (!course_form)
    {
        course_form = new CourseForm();
        course_form->setAttribute(Qt::WA_DeleteOnClose);

        connect(course_form, &CourseForm::destroyed, this, &MainWindow::on_course_window_closed);

        course_form->show();
    }
    else
    {
        course_form->raise();
        course_form->activateWindow();
    }
}

void MainWindow::open_section_form()
{
    if (!section_window)
    {
        section_window = new SectionWindow();
        section_window->setAttribute(Qt::WA_DeleteOnClose);

        connect(section_window, &SectionWindow::destroyed, this, &MainWindow::on_section_window_closed);

        section_window->show();
    }
    else
    {
        section_window->raise();
        section_window->activateWindow();
    }
}

void MainWindow::on_course_window_closed()
{
    course_form = nullptr;
    showing_professors = !showing_professors;
    update_table();
}

void MainWindow::on_section_window_closed()
{
    section_window = nullptr;
}

void MainWindow::onItemClicked(QTableWidgetItem *item)
{
    if(item->text().contains(QString("PROF")))
    {
        QMessageBox::StandardButton respuesta = QMessageBox::question
            (
            this,
            "Confirmar eliminación",
            "¿Estás seguro de que deseas eliminar este elemento?",
            QMessageBox::Yes | QMessageBox::No
            );

        if(respuesta == QMessageBox::Yes)
        {
            dm_instance.remove_professor(item->text().toStdString());
        }
    }

    if(item->text().contains(QString("COURSE")))
    {
        QMessageBox::StandardButton respuesta = QMessageBox::question
            (
            this,
            "Confirmar eliminación",
            "¿Estás seguro de que deseas eliminar este elemento?",
            QMessageBox::Yes | QMessageBox::No
            );

        if(respuesta == QMessageBox::Yes)
        {

            dm_instance.remove_course(item->text().toStdString());

        }
    }

    showing_professors = !showing_professors;
    update_table();
}
