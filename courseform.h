#ifndef COURSEFORM_H
#define COURSEFORM_H

#include <QMainWindow>

namespace Ui {
class CourseForm;
}

class CourseForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CourseForm(QWidget *parent = nullptr);
    ~CourseForm();

private:
    Ui::CourseForm *ui;
};

#endif // COURSEFORM_H
