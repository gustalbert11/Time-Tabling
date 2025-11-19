#ifndef FORMINSERTPROF_H
#define FORMINSERTPROF_H

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
#include "../include/data_manager.hpp"

namespace Ui {
class formInsertProf;
}

class formInsertProf : public QMainWindow
{
    Q_OBJECT

public:
    explicit formInsertProf(QWidget *parent = nullptr);
    ~formInsertProf();

private slots:
    void onSubmit();
    void onFieldReturnPressed();

private:void processForm();
    void setupConnections();
    Professor* prof;

    QLineEdit *lineEdit1;

    QSpinBox *spinbox1;
    QSpinBox *spinbox2;
    QSpinBox *spinbox3;

    QLineEdit *lineEdit5;


    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLabel *label6;
    QLabel *labelp;


    QComboBox *combox1;

    QPushButton *submitButton;

    Ui::formInsertProf *ui;
};

#endif // FORMINSERTPROF_H
