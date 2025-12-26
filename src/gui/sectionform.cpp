#include "sectionform.h"
#include "ui_sectionform.h"
#include <QDebug>

SectionForm::SectionForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SectionForm)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    this->setWindowTitle("Formulario de Seccion");
    this->setMinimumSize(600, 300);

    label1 = new QLabel("Escoge un profesor: ", this);
    combox1 = new QComboBox(this);

    auto& prof = dm_instance.get_professors();
    for(const auto& pair: prof)
    {
        id_prof.push_back(pair.first);
        combox1->addItem(QString::fromStdString(pair.second->get_name()));
    }

    label2 = new QLabel("Escoge una materia: ", this);
    combox2 = new QComboBox(this);

    auto& courses = dm_instance.get_courses();
    for(const auto& pair: courses)
    {
        id_course.push_back(pair.first);
        combox2->addItem(QString::fromStdString(pair.second->get_name()));
    }

    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(label1);
    layout1->addWidget(combox1);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(label2);
    layout2->addWidget(combox2);

    submitButton = new QPushButton(this);
    submitButton->setText("Enviar");
    QHBoxLayout *layoutl = new QHBoxLayout();
    layoutl->addWidget(submitButton);

    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layoutl);

    mainLayout->addStretch();

    this->setCentralWidget(centralWidget);
    setupConnections();
}

SectionForm::~SectionForm()
{
    delete ui;
}

void SectionForm::setupConnections()
{
    connect(submitButton, &QPushButton::clicked, this, &SectionForm::onSubmit);
    submitButton->setDefault(true);
}

void SectionForm::onFieldReturnPressed()
{
    onSubmit();
}

void SectionForm::onSubmit()
{
    processForm();
}

void SectionForm::processForm()
{
    auto section = std::make_unique<Section>();

    auto prof = dm_instance.get_professor(id_prof[combox1->currentIndex()]);
    auto course = dm_instance.get_course(id_course[combox2->currentIndex()]);

    section->set_professor(prof);
    section->set_course(course);

    prof->add_section(section.get());
    course->add_section(section.get());

    qDebug() << "\nProfesor: "
         << QString::fromStdString(prof->get_name())
         << " Seccion: "
         << QString::fromStdString(course->get_name())
         << "\n";
    dm_instance.add_section(std::move(section));

    this->close();
}
