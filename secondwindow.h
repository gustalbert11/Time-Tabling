#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSpacerItem>
#include <vector>
#include "include/Professor.hpp"
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr, Professor* p = nullptr);
    ~SecondWindow();

    QString getInput1() const;
    QString getInput2() const;
    QString getInput3() const;
    QString getInput4() const;
    QString getInput5() const;

private slots:
    void onSubmit();
    void onFieldReturnPressed();

private:
    void processForm();
    void setupConnections();
    Ui::SecondWindow *ui;
    Professor* prof;

    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;
    QLineEdit *lineEdit4;
    QLineEdit *lineEdit5;


    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;

    QPushButton *submitButton;
};

#endif // SECONDWINDOW_H
