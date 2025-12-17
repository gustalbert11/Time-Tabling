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
    ui->insertCourseButton->hide();
    ui->ShowInfoButton->setText("Mostrar Materias");
    // ui->tableInfo->setColumnCount(7);
    // ui->tableInfo->setHorizontalHeaderLabels(
    //     {"ID","Nombre", "Secciones", "Max horas diario", "Max horas consecutivo","Tipo de Pref","Descripcion Pref"}
    // );
    // ui->tableInfo->setColumnWidth(0, 150);
    // ui->tableInfo->setColumnWidth(1, 150);
    // ui->tableInfo->setColumnWidth(2, 150);
    // ui->tableInfo->setColumnWidth(3, 150);
    // ui->tableInfo->setColumnWidth(4, 150);
    // ui->tableInfo->setColumnWidth(5, 200);
    // ui->tableInfo->setColumnWidth(6, 200);
    ui->tableInfo->setColumnCount(6);
    ui->tableInfo->setHorizontalHeaderLabels(
        {"ID","Nombre", "Max horas diarias", "Max horas consecutivas","Tipo de preferencia","Descripcion preferencia"}
    );
    ui->tableInfo->setColumnWidth(0, 150);
    ui->tableInfo->setColumnWidth(1, 150);
    ui->tableInfo->setColumnWidth(2, 150);
    ui->tableInfo->setColumnWidth(3, 150);
    ui->tableInfo->setColumnWidth(4, 150);
    ui->tableInfo->setColumnWidth(5, 200);

    ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Asegurarnos de empezar siempre en la página 0 (el menú principal)
    ui->stackedWidget->setCurrentIndex(0);

    // CONEXIÓN MANUAL
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::go_to_next_window);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::go_to_previous_window);

    //connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::import_json);

    import_menu = new QMenu(this);
    
    // 3. Añadimos las acciones al menú
    QAction *act_json = import_menu->addAction("Importar Datos (JSON)");
    import_menu->addSeparator(); // Una linea separadora visual
    QAction *act_prof = import_menu->addAction("Importar Profesores (CSV)");
    QAction *act_course = import_menu->addAction("Importar Materias (CSV)");
    QAction *act_section = import_menu->addAction("Importar Secciones (CSV)");

    // 4. Conectamos las acciones a los slots correspondientes
    connect(act_json, &QAction::triggered, this, &MainWindow::import_json);
    connect(act_prof, &QAction::triggered, this, &MainWindow::import_professors_csv);
    connect(act_course, &QAction::triggered, this, &MainWindow::import_courses_csv);
    connect(act_section, &QAction::triggered, this, &MainWindow::import_sections_csv);

    // 5. Asignamos el menú al botón existente
    ui->importButton->setMenu(import_menu);
    
    // Opcional: Para que no parezca un botón normal, le agregamos una flechita visualmente
    // (Esto depende del estilo de tu SO, pero ayuda al usuario a saber que hay opciones)
    ui->importButton->setStyleSheet("QPushButton { text-align: center; }::menu-indicator { subcontrol-origin: padding; subcontrol-position: center right; }");

    //connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::export_json);

    export_menu = new QMenu(this);
    
    // 3. Añadimos las acciones al menú
    QAction *act_json2 = export_menu->addAction("Exportar Datos (JSON)");
    export_menu->addSeparator(); // Una linea separadora visual
    QAction *act_prof2 = export_menu->addAction("Exportar Profesores (CSV)");
    QAction *act_course2 = export_menu->addAction("Exportar Materias (CSV)");
    QAction *act_section2 = export_menu->addAction("Exportar Secciones (CSV)");

    // 4. Conectamos las acciones a los slots correspondientes
    connect(act_json2, &QAction::triggered, this, &MainWindow::export_json);
    connect(act_prof2, &QAction::triggered, this, &MainWindow::export_professors_csv);
    connect(act_course2, &QAction::triggered, this, &MainWindow::export_courses_csv);
    connect(act_section2, &QAction::triggered, this, &MainWindow::export_sections_csv);

    // 5. Asignamos el menú al botón existente
    ui->exportButton->setMenu(export_menu);
    
    // Opcional: Para que no parezca un botón normal, le agregamos una flechita visualmente
    // (Esto depende del estilo de tu SO, pero ayuda al usuario a saber que hay opciones)
    ui->exportButton->setStyleSheet("QPushButton { text-align: center; }::menu-indicator { subcontrol-origin: padding; subcontrol-position: center right; }");
    
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
            if (professor && professor->get_preference()) {
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

    // Nota: Pasamos 'true' para actualizar datos si el ID ya existe, o 'false' si prefieres solo ignorar.
    // Usamos 'true' para permitir correcciones masivas.
    bool ok = dm_instance.import_professors_from_csv(filename.toStdString(), true);

    if (ok) 
    {
        QMessageBox::information(this, "Éxito", "Profesores importados correctamente.");
        // Forzamos la vista de profesores
        showing_professors = false; // El update_table invierte esto, así que lo ponemos en false para que al invertir sea true
        update_table(); 
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "Error al leer el archivo CSV de profesores.");
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
        // Forzamos la vista de materias
        showing_professors = true; // El update_table invierte esto
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "Error al leer el archivo CSV de materias.");
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

void MainWindow::import_sections_csv()
{
    // ADVERTENCIA: Para importar secciones, los profesores y materias deben existir previamente.
    if (dm_instance.get_professor_count() == 0 || dm_instance.get_course_count() == 0) {
        QMessageBox::warning(this, "Advertencia", 
            "Para importar secciones, primero debes haber cargado Profesores y Materias.\n"
            "Esto asegura que las relaciones se creen correctamente.");
        // No retornamos, dejamos que el usuario intente si quiere, o podrías hacer return;
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
        // Las secciones no tienen vista propia en tu tabla principal actual (solo profes o materias),
        // así que refrescamos la vista actual.
        showing_professors = !showing_professors; // Hack para mantener la vista actual
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "Error al leer el archivo CSV de secciones.");
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
        showing_professors = !showing_professors;
        update_table();
    } 
    else 
    {
        QMessageBox::warning(this, "Error", "No se pudo procesar el archivo JSON.");
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
        // ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(prof->get_num_sections())));
        // ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(prof->get_max_daily_hours())));
        // ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::number(prof->get_max_consecutive_hours())));
        // ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(preference_type_to_string(prof->get_preference()->get_type()))));
        // ui->tableInfo->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(prof->get_preference()->get_description())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(prof->get_max_daily_hours())));
        ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(prof->get_max_consecutive_hours())));
        ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(preference_type_to_string(prof->get_preference()->get_type()))));
        ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(prof->get_preference()->get_description())));
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
        const auto &course = pair.second.get();

        ui->tableInfo->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(course->get_id())));
        ui->tableInfo->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(course->get_name())));
        ui->tableInfo->setItem(row, 2, new QTableWidgetItem(QString::number(course->get_level())));
        ui->tableInfo->setItem(row, 3, new QTableWidgetItem(QString::number(course->get_num_credits())));
        // ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::number(course->get_num_sections())));
        // ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::number(course->get_num_weekly_hours())));
        // ui->tableInfo->setItem(row, 6, new QTableWidgetItem(QString::number(course->get_max_daily_hours())));
        ui->tableInfo->setItem(row, 4, new QTableWidgetItem(QString::number(course->get_num_weekly_hours())));
        ui->tableInfo->setItem(row, 5, new QTableWidgetItem(QString::number(course->get_max_daily_hours())));
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
        // ui->tableInfo->setHorizontalHeaderLabels(
        // {"ID","Nombre", "Semestre", "U.C", "Secciones","Horas Semanales","Max Horas Diarias"}
        // );
        ui->tableInfo->setHorizontalHeaderLabels(
        {"ID","Nombre", "Semestre", "U.C","Horas Semanales","Max Horas Diarias"}
        );
        ui->tableInfo->setColumnWidth(0, 180);
        ui->tableInfo->setColumnWidth(1, 180);
        ui->tableInfo->setColumnWidth(2, 180);
        ui->tableInfo->setColumnWidth(3, 180);
        ui->tableInfo->setColumnWidth(4, 180);
        ui->tableInfo->setColumnWidth(5, 180);

        show_courses();

        showing_professors = false;
    } 
    else
    {
        ui->insertProfButton->show();
        ui->insertCourseButton->hide();
        ui->ShowInfoButton->setText("Mostrar Materias");
        // ui->tableInfo->setColumnCount(7);
        // ui->tableInfo->setHorizontalHeaderLabels(
        // {"ID","Nombre", "Secciones", "Max horas diario", "Max horas consecutivo","Tipo de Pref","Descripcion Pref"}
        // );
        // ui->tableInfo->setColumnWidth(0, 150);
        // ui->tableInfo->setColumnWidth(1, 150);
        // ui->tableInfo->setColumnWidth(2, 150);
        // ui->tableInfo->setColumnWidth(3, 150);
        // ui->tableInfo->setColumnWidth(4, 150);
        // ui->tableInfo->setColumnWidth(5, 200);
        // ui->tableInfo->setColumnWidth(6, 200);
        ui->tableInfo->setColumnCount(6);
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
    if(item->text().contains(QString("PROF")) && dm_instance.get_professor(item->text().toStdString()))
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

    if(item->text().contains(QString("COURSE")) && dm_instance.get_course(item->text().toStdString()))
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

    showing_professors = !showing_professors;
    update_table();
}
