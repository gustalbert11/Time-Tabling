#include "courseform.h"
#include "ui_courseform.h"

CourseForm::CourseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CourseForm)
{
    ui->setupUi(this);
}

CourseForm::~CourseForm()
{
    delete ui;
}
