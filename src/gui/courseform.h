#ifndef COURSEFORM_H
#define COURSEFORM_H

#include "professorform.h"

namespace Ui {
class CourseForm;
}

class CourseForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CourseForm(QWidget *parent = nullptr);
    ~CourseForm();

private slots:
    void onSubmit();
    void onFieldReturnPressed();

private:
    void processForm();
    void setupConnections();

    std::vector<QLabel*> labels;
    QLineEdit *line_edit;
    std::vector<QSpinBox*> spinboxes;

    QPushButton *submitButton;

    Ui::CourseForm *ui;
};

#endif // COURSEFORM_H
