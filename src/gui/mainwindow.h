#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "courseform.h"
#include "sectionwindow.h"
#include <QTableWidget>
#include <QMessageBox>
// [cite_start]#include <QMenu> //[cite: 1] Necesario para el menú desplegable
#include <QMenu>
#include "flow/flow_network.hpp"

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
    void go_to_next_window();
    void go_to_previous_window();

    void import_json();
    void import_professors_csv();
    void import_courses_csv();
    void import_sections_csv();

    void show_professors();
    void show_courses();
    
    void update_table();

    void open_prof_form();
    void open_course_form();
    void open_section_form();

    void on_professor_window_closed();
    void on_course_window_closed();
    void on_section_window_closed();

    void onItemClicked(QTableWidgetItem *item);

    void create_schedule();

private:
    Ui::MainWindow *ui;
    ProfessorForm* prof_form = nullptr;
    CourseForm* course_form = nullptr;
    SectionWindow* section_window = nullptr;
    bool showing_professors = true;

    QMenu *import_menu = nullptr;
};
#endif // MAINWINDOW_H
