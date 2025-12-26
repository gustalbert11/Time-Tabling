#ifndef SECTIONFORM_H
#define SECTIONFORM_H

#include "professorform.h"

namespace Ui 
{
class SectionForm;
}

class SectionForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit SectionForm(QWidget *parent = nullptr);
    ~SectionForm();

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
    Ui::SectionForm *ui;
};

#endif // SECTIONFORM_H
