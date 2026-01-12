#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Time Tabling");

    ui->showInfoButton->hide();

    // 1. Crear el grupo de botones
    view_group = new QButtonGroup(this);

    // 2. Instanciar los RadioButtons
    radio_prof = new QRadioButton("Profesores", this);
    radio_course = new QRadioButton("Materias", this);
    radio_sect = new QRadioButton("Secciones", this);

    // 3. Añadirlos al grupo asignando el ID según el enum EntityType 
    view_group->addButton(radio_prof, EntityType::PROFESSOR);
    view_group->addButton(radio_course, EntityType::COURSE);
    view_group->addButton(radio_sect, EntityType::SECTION);

    // 4. Conectar la señal idClicked al slot
    connect(view_group, &QButtonGroup::idClicked, this, &MainWindow::on_view_changed);

    // 5. Añadirlos a la interfaz (por ejemplo, al layout vertical que ya tienes) [cite: 5, 6]
    ui->verticalLayout_3->addWidget(radio_prof);
    ui->verticalLayout_3->addWidget(radio_course);
    ui->verticalLayout_3->addWidget(radio_sect);

    // 6. Definir el estado inicial
    radio_prof->setChecked(true);
    update_table();

    ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Asegurarnos de empezar siempre en la página 0 (el menú principal)
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::go_to_next_window);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::go_to_previous_window);

    import_menu = new QMenu(this);
    
    QAction *act_json = import_menu->addAction("Importar Datos (JSON)");
    import_menu->addSeparator();
    QAction *act_prof = import_menu->addAction("Importar Profesores (CSV)");
    QAction *act_course = import_menu->addAction("Importar Materias (CSV)");
    QAction *act_section = import_menu->addAction("Importar Secciones (CSV)");

    connect(act_json, &QAction::triggered, this, &MainWindow::import_json);
    connect(act_prof, &QAction::triggered, this, &MainWindow::import_professors_csv);
    connect(act_course, &QAction::triggered, this, &MainWindow::import_courses_csv);
    connect(act_section, &QAction::triggered, this, &MainWindow::import_sections_csv);

    ui->importButton->setMenu(import_menu);
    
    // Opcional: Para que no parezca un botón normal, le agregamos una flechita visualmente
    ui->importButton->setStyleSheet("QPushButton { text-align: center; }::menu-indicator { subcontrol-origin: padding; subcontrol-position: center right; }");

    export_menu = new QMenu(this);
    
    QAction *act_json2 = export_menu->addAction("Exportar Datos (JSON)");
    export_menu->addSeparator();
    QAction *act_prof2 = export_menu->addAction("Exportar Profesores (CSV)");
    QAction *act_course2 = export_menu->addAction("Exportar Materias (CSV)");
    QAction *act_section2 = export_menu->addAction("Exportar Secciones (CSV)");

    connect(act_json2, &QAction::triggered, this, &MainWindow::export_json);
    connect(act_prof2, &QAction::triggered, this, &MainWindow::export_professors_csv);
    connect(act_course2, &QAction::triggered, this, &MainWindow::export_courses_csv);
    connect(act_section2, &QAction::triggered, this, &MainWindow::export_sections_csv);

    ui->exportButton->setMenu(export_menu);
    
    // Opcional: Para que no parezca un botón normal, le agregamos una flechita visualmente
    ui->exportButton->setStyleSheet("QPushButton { text-align: center; }::menu-indicator { subcontrol-origin: padding; subcontrol-position: center right; }");
    
    connect(ui->insertProfButton, &QPushButton::clicked, this, &MainWindow::open_prof_form);

    connect(ui->showInfoButton, &QPushButton::clicked, this, &MainWindow::update_table);

    connect(ui->insertCourseButton, &QPushButton::clicked, this, &MainWindow::open_course_form);

    connect(ui->tableInfo, &QTableWidget::itemClicked, this, &MainWindow::onItemClicked);

    connect(ui->insertSecButton, &QPushButton::clicked, this, &MainWindow::open_section_form);

    connect(ui->scheduleButton, &QPushButton::clicked, this, &MainWindow::create_schedule);

    connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() 
    {
    auto_save_data();
    });
    //connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::auto_save_data);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::go_to_next_window()
{
    int it = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(it + 1);
    dm_instance.clear_all_data();
}
void MainWindow::go_to_previous_window()
{
    int it = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(it - 1);
    auto_save_data();
}

void MainWindow::on_view_changed(int id)
{
    current_entity_type = static_cast<EntityType>(id);
     
    update_table();
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

    bool ok = dm_instance.import_from_json(filename.toStdString(), true);

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "El archivo JSON fue importado correctamente.");
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo procesar el archivo JSON.");
    }
}
void MainWindow::import_professors_csv()
{
    QString filename = QFileDialog::getOpenFileName(
        this, "Importar Profesores", "", "CSV Files (*.csv);;All Files (*)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.import_professors_from_csv(filename.toStdString(), true);

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Profesores importados correctamente.");
        update_table(); 
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "Error al leer el archivo CSV de profesores.");
    }
}
void MainWindow::import_courses_csv()
{
    QString filename = QFileDialog::getOpenFileName(
        this, "Importar Materias", "", "CSV Files (*.csv);;All Files (*)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.import_courses_from_csv(filename.toStdString(), true);

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Materias importadas correctamente.");
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "Error al leer el archivo CSV de materias.");
    }
}
void MainWindow::import_sections_csv()
{
    // ADVERTENCIA: Para importar secciones, los profesores y materias deben existir previamente.
    if (dm_instance.get_professor_count() == 0 || 
        dm_instance.get_course_count() == 0) 
    {
        QMessageBox::warning(this, "Advertencia", 
            "Para importar secciones, primero debes haber cargado Profesores y Materias.\n"
            "Esto asegura que las relaciones se creen correctamente.");
        return;
    }

    QString filename = QFileDialog::getOpenFileName(
        this, "Importar Secciones", "", "CSV Files (*.csv);;All Files (*)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.import_sections_from_csv(filename.toStdString(), true);

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Secciones importadas correctamente.");
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "Error al leer el archivo CSV de secciones.");
    }
}

void MainWindow::export_json()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Guardar archivo JSON",
        "",
        "JSON Files (*.json)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.export_to_json(filename.toStdString());

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "El archivo JSON fue exportado correctamente.");
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo exportar el archivo JSON.");
    }
}
void MainWindow::export_professors_csv()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Exportar Profesores", "", "CSV Files (*.csv);;All Files (*)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.export_professors_to_csv(filename.toStdString());

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Profesores exportados correctamente.");
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo exportar el archivo CSV de profesores.");
    }
}
void MainWindow::export_courses_csv()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Exportar Materias", "", "CSV Files (*.csv);;All Files (*)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.export_courses_to_csv(filename.toStdString());

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Materias exportadas correctamente.");
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo exportar el archivo CSV de materias.");
    }
}
void MainWindow::export_sections_csv()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Exportar Secciones", "", "CSV Files (*.csv);;All Files (*)"
    );

    if (filename.isEmpty()) 
    {
        return;
    }

    bool ok = dm_instance.export_sections_to_csv(filename.toStdString());

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Secciones exportadas correctamente.");
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo exportar el archivo CSV de secciones.");
    }
}

void MainWindow::auto_save_data(EntityType type)
{
    std::string json_backup = "autosave_data.json";
    
    bool success = dm_instance.export_to_json(json_backup);
    
    if (!success) 
    {
        qWarning() << "Error en el autoguardado.";
        return;
    } 

    qDebug() << "Autoguardado exitoso en:" << QString::fromStdString(json_backup);

    std::string csv_backup;

    switch (type) 
    {
        case EntityType::PROFESSOR:
            csv_backup = "autosave_professors.csv";
            success = dm_instance.export_professors_to_csv(csv_backup);
            if (!success) 
            {
                qWarning() << "Error en el autoguardado de Profesores.";
                return;
            }
            qDebug() << "Autoguardado de Profesores completado.";
            break;
        
        case EntityType::COURSE:
            csv_backup = "autosave_courses.csv";
            success = dm_instance.export_courses_to_csv(csv_backup);
            if (!success) 
            {
                qWarning() << "Error en el autoguardado de Materias.";
                return;
            }
            qDebug() << "Autoguardado de Materias completado.";
            break;
        
        case EntityType::SECTION:
            csv_backup = "autosave_sections.csv";
            success = dm_instance.export_sections_to_csv(csv_backup);
            if (!success) 
            {
                qWarning() << "Error en el autoguardado de Secciones.";
                return;
            }
            qDebug() << "Autoguardado de Secciones completado.";
            break;
        
        default:
            qDebug() << "Autoguardado de todos los datos completado.";
            break;
    }
}

// void MainWindow::auto_save_data()
// {
//     std::string json_backup = "autosave_data.json";
    
//     bool success = dm_instance.export_to_json(json_backup);
    
//     if (!success) 
//     {
//         qWarning() << "Error en el autoguardado.";
//         return;
//     } 

//     qDebug() << "Autoguardado exitoso en:" << QString::fromStdString(json_backup);
// }

void MainWindow::show_professors()
{
    const auto& professors = dm_instance.get_professors();

    ui->tableInfo->setRowCount(static_cast<int>(professors.size()));

    int row = 0;
    for (const auto &pair : professors)
    {
        const auto &prof = pair.second.get();

        ui->tableInfo->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(prof->get_id())));
        ui->tableInfo->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(prof->get_name())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(prof->get_max_daily_hours())));
        ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(prof->get_max_consecutive_hours())));
        ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(preference_type_to_string(prof->get_preference()->get_type()))));
        ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(prof->get_preference()->get_description())));
        row++;
    }
}
void MainWindow::show_courses()
{
    const auto& courses = dm_instance.get_courses();

    ui->tableInfo->setRowCount(static_cast<int>(courses.size()));       

    int row = 0;
    for (const auto &pair : courses)
    {
        const auto &course = pair.second.get();

        ui->tableInfo->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(course->get_id())));
        ui->tableInfo->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(course->get_name())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(course->get_level())));
        ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(course->get_num_credits())));
        ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::number(course->get_num_weekly_hours())));
        ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::number(course->get_max_daily_hours())));
        row++;
    }
}
void MainWindow::show_sections()
{
    const auto& sections = dm_instance.get_sections();

    ui->tableInfo->setRowCount(static_cast<int>(sections.size()));       

    int row = 0;
    for (const auto &pair : sections)
    {
        const auto &section = pair.second.get();

        ui->tableInfo->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(section->get_id())));
        ui->tableInfo->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(section->get_course()->get_id())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(section->get_professor()->get_id())));
        row++;
    }
}

void MainWindow::update_table()
{
    ui->tableInfo->clear();
    ui->tableInfo->setRowCount(0);
    ui->tableInfo->setColumnCount(0);

    switch (current_entity_type) 
    {
        case PROFESSOR:
            ui->insertProfButton->show();
            ui->insertCourseButton->hide();
            ui->insertSecButton->hide();
            ui->tableInfo->setColumnCount(6);
            ui->scheduleButton->hide();
            ui->tableInfo->setHorizontalHeaderLabels(
                {"ID","Nombre", "Max horas diarias", "Max horas consecutivas","Tipo de Preferencia","Descripcion preferencia"}
            );
            ui->tableInfo->setColumnWidth(0, 150);
            ui->tableInfo->setColumnWidth(1, 150);
            ui->tableInfo->setColumnWidth(2, 150);
            ui->tableInfo->setColumnWidth(3, 150);
            ui->tableInfo->setColumnWidth(4, 150);
            ui->tableInfo->setColumnWidth(5, 200);
            show_professors();
            break;

        case COURSE:
            ui->insertProfButton->hide();
            ui->insertCourseButton->show();
            ui->insertSecButton->hide();
            ui->scheduleButton->hide();
            ui->tableInfo->setColumnCount(6);
            ui->tableInfo->setHorizontalHeaderLabels(
                {"ID","Nombre", "Nivel", "U.C","Horas Semanales","Max Horas Diarias"}
            );
            ui->tableInfo->setColumnWidth(0, 180);
            ui->tableInfo->setColumnWidth(1, 180);
            ui->tableInfo->setColumnWidth(2, 180);
            ui->tableInfo->setColumnWidth(3, 180);
            ui->tableInfo->setColumnWidth(4, 180);
            ui->tableInfo->setColumnWidth(5, 180);
            show_courses();
            break;

        case SECTION:
            ui->insertProfButton->hide();
            ui->insertCourseButton->hide();
            ui->insertSecButton->show();
            ui->scheduleButton->show();
            ui->tableInfo->setColumnCount(3);
            ui->tableInfo->setHorizontalHeaderLabels(
                {"ID Sección","ID Materia", "ID Profesor"}
            );
            ui->tableInfo->setColumnWidth(0, 200);
            ui->tableInfo->setColumnWidth(1, 200);
            ui->tableInfo->setColumnWidth(2, 200);
            show_sections();
            break;

        default:
            break;
    }
}

void MainWindow::open_prof_form()
{
    if (!prof_form) 
    {
        prof_form = new ProfessorForm();
        prof_form->setAttribute(Qt::WA_DeleteOnClose);

        connect(prof_form, &ProfessorForm::destroyed, this, &MainWindow::on_professor_window_closed);

        prof_form->show();
    }
    else 
    {
        prof_form->raise();
        prof_form->activateWindow();
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
    if (!section_form)
    {
        section_form = new SectionForm();
        section_form->setAttribute(Qt::WA_DeleteOnClose);

        connect(section_form, &SectionForm::destroyed, this, &MainWindow::on_section_window_closed);

        section_form->show();
    }
    else
    {
        section_form->raise();
        section_form->activateWindow();
    }
}

void MainWindow::on_professor_window_closed()
{
    prof_form = nullptr;
    update_table();
    auto_save_data(EntityType::PROFESSOR);
    //auto_save_data();
}
void MainWindow::on_course_window_closed()
{
    course_form = nullptr;
    update_table();
    auto_save_data(EntityType::COURSE);
    //auto_save_data();
}
void MainWindow::on_section_window_closed()
{
    section_form = nullptr;
    update_table();
    auto_save_data(EntityType::SECTION);
    //auto_save_data();
}

void MainWindow::onItemClicked(QTableWidgetItem *item)
{
    if(item->text().contains(QString("PROF")) && 
       dm_instance.get_professor(item->text().toStdString()))
    {
        QMessageBox::StandardButton answer = QMessageBox::question
            (
            this,
            "Confirmar eliminación",
            "¿Estás seguro de que deseas eliminar este elemento?",
            QMessageBox::Yes | QMessageBox::No
            );

        if(answer == QMessageBox::Yes)
        {
            dm_instance.remove_professor(item->text().toStdString());
        }
    }

    if(item->text().contains(QString("COURSE")) && 
       dm_instance.get_course(item->text().toStdString()))
    {
        QMessageBox::StandardButton answer = QMessageBox::question
            (
            this,
            "Confirmar eliminación",
            "¿Estás seguro de que deseas eliminar este elemento?",
            QMessageBox::Yes | QMessageBox::No
            );

        if(answer == QMessageBox::Yes)
        {

            dm_instance.remove_course(item->text().toStdString());

        }
    }

    update_table();
}

void MainWindow::create_schedule()
{
    // ADVERTENCIA: Los profesores, materias y secciones deben existir previamente.
    if (dm_instance.get_professor_count() == 0 || 
        dm_instance.get_course_count() == 0 ||
        dm_instance.get_section_count() == 0)
    {
        QMessageBox::warning(this, "Advertencia", 
            "Primero deben existir Profesores, Materias y Secciones.\n"
            "Esto asegura que el algoritmo de asignación funcione correctamente.");
        return;
    }
    
    std::ofstream out("resultado_schedule.txt");  // 📄 archivo de salida

    // Inicializar red
    fn_instance.init();

    // 3. Ejecutar algoritmo
    if (fn_instance.solve_min_cost_max_flow()) 
    {
        out << "✅ Algoritmo completado\n";

        auto schedule = fn_instance.get_final_schedule();
        for (const auto& entry : schedule) 
        {
            out << "\n📅 ASIGNACIÓN FINAL:\n";
            out << "   Profesor: " << entry.professor_name << "\n";
            out << "   Materia: " << entry.course_name << "\n";
            out << "   Día: " << day_to_string(entry.day) << "\n";
            out << "   Horario: " << entry.start_hour << ":00-" << entry.end_hour << ":00\n";

            auto professor = dm_instance.get_professor(entry.professor_id);
            if (professor && 
                professor->get_preference()) 
            {
                auto pref = professor->get_preference();
                bool day_ok = pref->get_days().contains(entry.day);
                bool hour_ok = false;

                for (const auto& hour_range : pref->get_hours()) 
                {
                    if (entry.start_hour >= hour_range.first && entry.end_hour <= hour_range.second) {
                        hour_ok = true;
                        break;
                    }
                }
            }
        }
    }

    out.close(); // opcional, se cierra solo
}
