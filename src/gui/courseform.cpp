#include "courseform.h"
#include "ui_courseform.h" 
#include <QString>

CourseForm::CourseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CourseForm)
{
    ui->setupUi(this);

    // Crear widget central y layout principal
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar la ventana
    this->setWindowTitle("Formulario de Datos");
    this->setMinimumSize(600, 300);

    labels.push_back(new QLabel("Nombre de la Materia", this));
    line_edit = new QLineEdit(this);

    labels.push_back(new QLabel("Nivel de la materia", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[0]->setRange(1, MAX_NUM_LEVEL);

    labels.push_back(new QLabel("Numero de UC", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[1]->setRange(1, MAX_NUM_CREDITS);

    labels.push_back(new QLabel("Numero de secciones", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[2]->setRange(1, COURSE_MAX_NUM_SECTIONS);

    labels.push_back(new QLabel("Horas semanales", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[3]->setRange(4, MAX_WEEKLY_HOURS);

    labels.push_back(new QLabel("Horas maximas diarias", this));
    spinboxes.push_back(new QSpinBox(this));
    spinboxes[4]->setRange(2, MAX_DAILY_HOURS);

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

    // Configurar el widget central
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
    onSubmit(); // Último campo: enviar
}

void CourseForm::onSubmit()
{
    processForm();
}

void CourseForm::processForm()
{
    auto cours = std::make_unique<Course>();
    QString data1 = line_edit->text();
    cours->set_name(data1.toStdString());

    uint data2 = static_cast<uint>(spinboxes[0]->value());
    cours->set_level(data2);

    uint data3 = static_cast<uint>(spinboxes[1]->value());
    cours->set_num_credits(data3);

    uint data4 = static_cast<uint>(spinboxes[2]->value());
    cours->set_num_sections(data4);

    uint data5 = static_cast<uint>(spinboxes[3]->value());
    cours->set_num_weekly_hours(data5);

    uint data6 = static_cast<uint>(spinboxes[4]->value());
    cours->set_max_daily_hours(data6);

    if (data1.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor complete los campos obligatorios");
        return;
    }

    dm_instance.add_course(std::move(cours));
    this->close();
}

CourseForm::~CourseForm()
{
    delete ui;
}
