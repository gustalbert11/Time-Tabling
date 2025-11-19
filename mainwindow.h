#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secondwindow.h"
#include "include/Professor.hpp"

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
    void avanzar_ventana();

    void volver_ventana();

    void show_professor_form();

    void avanzar_pg2();

    void on_professor_window_closed();


private:
    Ui::MainWindow *ui;
    SecondWindow* professorWindow = nullptr;
    Professor p;

};
#endif // MAINWINDOW_H
