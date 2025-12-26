#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "courseform.h"
#include "sectionform.h"
#include <QTableWidget>
// [cite_start]#include <QMenu> //[cite: 1] Necesario para el menú desplegable
#include <QRadioButton>
#include <QButtonGroup>
#include <QMenu>
#include "flow/flow_network.hpp"

QT_BEGIN_NAMESPACE
namespace Ui 
{
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

    void on_view_changed(int id);

    void import_json();
    void import_professors_csv();
    void import_courses_csv();
    void import_sections_csv();
    
    void export_json();
    void export_professors_csv();
    void export_courses_csv();
    void export_sections_csv();

    void show_professors();
    void show_courses();
    void show_sections();
    
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
    SectionForm* section_form = nullptr;
    bool showing_professors = true;
    EntityType current_entity_type = EntityType::PROFESSOR;

    QButtonGroup *view_group;
    QRadioButton *radio_prof;
    QRadioButton *radio_course;
    QRadioButton *radio_sect;

    QMenu *import_menu = nullptr;
    QMenu *export_menu = nullptr;

};
#endif // MAINWINDOW_H
