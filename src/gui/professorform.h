#ifndef PROFESSORFORM_H
#define PROFESSORFORM_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSpacerItem>
#include <QPushButton>
#include <QMessageBox>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "core/data_manager.hpp"

namespace Ui {
class ProfessorForm;
}

class ProfessorForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfessorForm(QWidget *parent = nullptr);
    ~ProfessorForm();

private slots:
    void onSubmit();
    void onFieldReturnPressed();
    void combox1_current_text_changed();
private:
    void processForm();
    void setupConnections();
    Professor* prof;

    QLineEdit *lineEdit1;

    QSpinBox *spinbox1;
    QSpinBox *spinbox2;
    QSpinBox *spinbox3;
    QSpinBox *spinbox4;
    QSpinBox *spinbox5;

    QLineEdit *lineEdit5;


    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLabel *label6;
    QLabel *label7;
    QLabel *labelp;


    QComboBox *combox1;


    std::vector<QCheckBox*> checkBoxes;

    QPushButton *submitButton;

    Ui::ProfessorForm *ui;
};

#endif // PROFESSORFORM_H
