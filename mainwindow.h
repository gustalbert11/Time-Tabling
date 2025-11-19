#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "forminsertprof.h"

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

    void update_prof_table();

    void open_prof_form();

    void on_professor_window_closed();

private:
    Ui::MainWindow *ui;
    ProfessorForm* prof_form = nullptr;
};
#endif // MAINWINDOW_H
