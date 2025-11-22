#ifndef SECTIONWINDOW_H
#define SECTIONWINDOW_H

#include <QMainWindow>
#include "forminsertprof.h"

namespace Ui {
class SectionWindow;
}

class SectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SectionWindow(QWidget *parent = nullptr);
    ~SectionWindow();

private slots:
    void onSubmit();
    void onFieldReturnPressed();
private:
    void processForm();
    void setupConnections();

    std::vector<std::string> id_prof;
    std::vector<std::string> id_course;

    QLabel* label1;
    QLabel* label2;

    QComboBox* combox1;
    QComboBox* combox2;

    QPushButton* submitButton;
    Ui::SectionWindow *ui;
};

#endif // SECTIONWINDOW_H
