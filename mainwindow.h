#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "courseform.h"
#include "sectionwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void avanzar_ventana();  // Cambia el nombre del slot

    void volver_ventana();

    void import_json();

    void show_professors();

    void open_prof_form();

    void on_course_window_closed();

    void on_professor_window_closed();

    void on_section_window_closed();

    void show_courses();

    void update_table();

    void open_course_form();

    void open_section_form();

private:
    Ui::MainWindow *ui;
    ProfessorForm* prof_form = nullptr;
    CourseForm* course_form = nullptr;
    SectionWindow* section_window = nullptr;
    bool showing_professors = true;
};
#endif // MAINWINDOW_H
