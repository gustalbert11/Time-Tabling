#include "sectionwindow.h"
#include "ui_sectionwindow.h"
#include "core/data_manager.hpp"
#include <QDebug>

SectionWindow::SectionWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SectionWindow)
{
    ui->setupUi(this);

    // Crear widget central y layout principal
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar la ventana
    this->setWindowTitle("Formulario de Datos");
    this->setMinimumSize(600, 300);

    label1 = new QLabel("Escoga un profesor", this);
    combox1 = new QComboBox(this);

    auto& prof = dm_instance.get_professors();
    for(const auto& pair: prof)
    {
        id_prof.push_back(pair.first);
        combox1->addItem(QString::fromStdString(pair.second->get_name()));
    }

    label2 = new QLabel("Escoga una materia", this);
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

    // Configurar el widget central
    this->setCentralWidget(centralWidget);
    setupConnections();
}

SectionWindow::~SectionWindow()
{
    delete ui;
}

void SectionWindow::setupConnections()
{
    connect(submitButton, &QPushButton::clicked, this, &SectionWindow::onSubmit);
    submitButton->setDefault(true);
}

void SectionWindow::onFieldReturnPressed()
{
    onSubmit();
}

void SectionWindow::onSubmit()
{
    processForm();
}


void SectionWindow::processForm()
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
