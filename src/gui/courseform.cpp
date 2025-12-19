#include "courseform.h"
#include "ui_courseform.h" 
#include <QString>

CourseForm::CourseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CourseForm)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar la ventana
    this->setWindowTitle("Formulario de Materia");
    this->setMinimumSize(600, 300);

    labels.push_back(new QLabel("Nombre: ", this));
    line_edit = new QLineEdit(this);

    labels.push_back(new QLabel("Nivel: ", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[0]->setRange(1, MAX_NUM_LEVELS);

    labels.push_back(new QLabel("Numero de UC: ", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[1]->setRange(1, MAX_NUM_CREDITS);

    labels.push_back(new QLabel("Horas semanales: ", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[2]->setRange(4, MAX_WEEKLY_HOURS);
    spinboxes[2]->setSingleStep(2);

    labels.push_back(new QLabel("Maximo de horas diarias: ", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[3]->setRange(2, MAX_DAILY_HOURS);
    spinboxes[3]->setSingleStep(2);

    std::vector<QHBoxLayout*> layouts;
    layouts.push_back(new QHBoxLayout());
    layouts[0]->addWidget(labels[0]);
    layouts[0]->addWidget(line_edit);
    mainLayout->addLayout(layouts[0]);

    for(int i = 0; i < spinboxes.size(); i++)
    {
        layouts.push_back(new QHBoxLayout());
        layouts[i + 1]->addWidget(labels[i + 1]);
        layouts[i + 1]->addWidget(spinboxes[i]);
        mainLayout->addLayout(layouts[i + 1]);
    }

    submitButton = new QPushButton(this);
    submitButton->setText("Enviar");
    layouts.push_back(new QHBoxLayout());
    layouts[layouts.size() - 1]->addWidget(submitButton);
    mainLayout->addLayout(layouts[layouts.size() - 1]);

    //Espaciado
    mainLayout->addStretch();

    this->setCentralWidget(centralWidget);
    setupConnections();
}

void CourseForm::setupConnections()
{
    //connect(line_edit, &QLineEdit::returnPressed, this, &CourseForm::onFieldReturnPressed);
    connect(submitButton, &QPushButton::clicked, this, &CourseForm::onSubmit);
    submitButton->setDefault(true);
}

void CourseForm::onFieldReturnPressed()
{
    onSubmit();
}

void CourseForm::onSubmit()
{
    processForm();
}

void CourseForm::processForm()
{
    auto course = std::make_unique<Course>();
    QString data1 = line_edit->text();
    course->set_name(data1.toStdString());

    uint data2 = static_cast<uint>(spinboxes[0]->value());
    course->set_level(data2);

    uint data3 = static_cast<uint>(spinboxes[1]->value());
    course->set_num_credits(data3);

    uint data5 = static_cast<uint>(spinboxes[2]->value());
    course->set_num_weekly_hours(data5);

    uint data6 = static_cast<uint>(spinboxes[3]->value());
    course->set_max_daily_hours(data6);

    if (data1.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor complete los campos obligatorios");
        return;
    }

    dm_instance.add_course(std::move(course));
    this->close();
}

CourseForm::~CourseForm()
{
    delete ui;
}
