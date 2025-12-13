#include "professorform.h"
#include "ui_professorform.h"

ProfessorForm::ProfessorForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProfessorForm)
{
    ui->setupUi(this);

    // Crear widget central y layout principal
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar la ventana
    this->setWindowTitle("Formulario de Datos");
    this->setMinimumSize(600, 300);

    // Crear los campos de entrada y etiquetas
    label1 = new QLabel("Nombre: ", this);
    lineEdit1 = new QLineEdit(this);

    // label2 = new QLabel("Numero de Secciones:", this);
    // spinbox1 = new QSpinBox(this);
    // spinbox1->setRange(1,PROF_MAX_NUM_SECTIONS);

    label3 = new QLabel("Maximo de horas diarias:", this);
    spinbox2 = new QSpinBox(this);
    spinbox2->setRange(2,MAX_DAILY_HOURS);

    label4 = new QLabel("Maximo de horas consecutivas:", this);
    spinbox3 = new QSpinBox(this);
    spinbox3->setRange(2,MAX_CONSECUTIVE_HOURS);

    labelp = new QLabel("Preferencias\n", this);

    label5 = new QLabel("Descripcion:", this);
    lineEdit5 = new QLineEdit(this);

    label6 = new QLabel("Tipo de preferencia:", this);
    combox1 = new QComboBox(this);

    combox1->addItem("DAYS & HOURS",    QVariant(PreferenceType::DAYS_HOURS));
    combox1->addItem("DAYS",       QVariant(PreferenceType::DAYS));
    combox1->addItem("NO PREFERENCE", QVariant(PreferenceType::NO_PREFERENCE));
    combox1->addItem("HOURS",      QVariant(PreferenceType::HOURS));


    checkBoxes.push_back(new QCheckBox("MONDAY",this));
    checkBoxes.push_back(new QCheckBox("TUESDAY",this));
    checkBoxes.push_back(new QCheckBox("WEDNESDAY",this));
    checkBoxes.push_back(new QCheckBox("THURSDAY",this));
    checkBoxes.push_back(new QCheckBox("FRIDAY",this));

    label7 = new QLabel("Hora de Inicio - Hora Final (Formato 24h)");
    spinbox4 = new QSpinBox(this);
    spinbox4->setRange(MIN_START_HOUR, MAX_END_HOUR - 2);
    spinbox5 = new QSpinBox(this);
    spinbox5->setRange(MIN_START_HOUR + 2, MAX_END_HOUR);

    // Crear layout para cada campo
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(label1);
    layout1->addWidget(lineEdit1);

    // QHBoxLayout *layout2 = new QHBoxLayout();
    // layout2->addWidget(label2);
    // layout2->addWidget(spinbox1);

    QHBoxLayout *layout3 = new QHBoxLayout();
    layout3->addWidget(label3);
    layout3->addWidget(spinbox2);

    QHBoxLayout *layout4 = new QHBoxLayout();
    layout4->addWidget(label4);
    layout4->addWidget(spinbox3);

    QHBoxLayout *layoutp = new QHBoxLayout();
    layoutp->addWidget(labelp);

    QHBoxLayout *layout5 = new QHBoxLayout();
    layout5->addWidget(label5);
    layout5->addWidget(lineEdit5);


    QHBoxLayout *layout6 = new QHBoxLayout();
    layout6->addWidget(label6);
    layout6->addWidget(combox1);

    QHBoxLayout *layout7 = new QHBoxLayout();
    layout7->addWidget(label6);
    layout7->addWidget(combox1);

    QHBoxLayout *layout8 = new QHBoxLayout();
    for(int i = 0; i < checkBoxes.size(); i++)
    {
        layout8->addWidget(checkBoxes[i]);
    }

    QHBoxLayout *layout9 = new QHBoxLayout();
    layout9->addWidget(label7);
    layout9->addWidget(spinbox4);
    layout9->addWidget(spinbox5);


    submitButton = new QPushButton(this);
    submitButton->setText("Enviar");
    QHBoxLayout *layoutl = new QHBoxLayout();
    layoutl->addWidget(submitButton);

    // Agregar los layouts al layout principal
    mainLayout->addLayout(layout1);
    //mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);
    mainLayout->addLayout(layoutp);
    mainLayout->addLayout(layout5);
    mainLayout->addLayout(layout6);
    mainLayout->addLayout(layout7);
    mainLayout->addLayout(layout8);
    mainLayout->addLayout(layout9);
    mainLayout->addLayout(layoutl);

    // Agregar un espacio elástico al final para que los campos no se expandan demasiado
    mainLayout->addStretch();

    // Configurar el widget central
    this->setCentralWidget(centralWidget);
    setupConnections();
}

void ProfessorForm::setupConnections()
{
    // Conectar Enter en cada campo
    connect(lineEdit1, &QLineEdit::returnPressed, this, &ProfessorForm::onFieldReturnPressed);
    connect(lineEdit5, &QLineEdit::returnPressed, this, &ProfessorForm::onFieldReturnPressed);
    connect(combox1, &QComboBox::currentTextChanged, this, &ProfessorForm::combox1_current_text_changed);
    connect(spinbox4, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProfessorForm::start_hour_spin_box_changed);
    connect(spinbox5, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProfessorForm::end_hour_spin_box_changed);

    connect(submitButton, &QPushButton::clicked, this, &ProfessorForm::onSubmit);
    submitButton->setDefault(true);
}

ProfessorForm::~ProfessorForm()
{
    delete ui;
}

void ProfessorForm::onFieldReturnPressed()
{
   onSubmit(); // Último campo: enviar
}

void ProfessorForm::combox1_current_text_changed()
{
    if(combox1->currentText() != "DAYS" && combox1->currentText() != "DAYS & HOURS")
    {
        for (int i = 0; i < checkBoxes.size(); ++i)
        {
            checkBoxes[i]->hide();
        }
    }
    else
    {
        for (int i = 0; i < checkBoxes.size(); ++i)
        {
            checkBoxes[i]->show();
        }
    }
    if(combox1->currentText() != "HOURS" && combox1->currentText() != "DAYS & HOURS")
    {
        label7->hide();
        spinbox4->hide();
        spinbox5->hide();
    }
    else
    {
        label7->show();
        spinbox4->show();
        spinbox5->show();
    }
}

void ProfessorForm::start_hour_spin_box_changed()
{
    int start_hour = spinbox4->value();
    int min_end_hour = start_hour + 2;
    spinbox5->setMinimum(min_end_hour);
    if (spinbox5->value() < min_end_hour) 
    {
        spinbox5->setValue(min_end_hour);
    }
}

void ProfessorForm::end_hour_spin_box_changed()
{
    int end_hour = spinbox5->value();
    int max_start_hour = end_hour - 2;
    spinbox4->setMaximum(max_start_hour);
    if (spinbox4->value() > max_start_hour) 
    {
        spinbox4->setValue(max_start_hour);
    }
}

void ProfessorForm::onSubmit()
{
    processForm();
}

void ProfessorForm::processForm()
{
    // Obtener y procesar todos los
    auto prof = std::make_unique<Professor>();

    QString data1 = lineEdit1->text();
    prof->set_name(data1.toStdString());

    // uint data2 = static_cast<uint>(spinbox1->value());
    // prof->set_num_sections(data2);

    uint data3 = static_cast<uint>(spinbox2->value());
    prof->set_max_daily_hours(data3);

    uint data4 = static_cast<uint>(spinbox3->value());
    prof->set_max_consecutive_hours(data4);

    auto pref = std::make_unique<Preference>();
    QString data5 = lineEdit5->text();
    pref->set_description(data5.toStdString());

    QString data6 = combox1->currentText();

    pref->set_type(string_to_preference_type(data6.toStdString()));

    for (int i = 0; i < checkBoxes.size(); ++i)
    {
        if(checkBoxes[i]->isChecked())
        {
            pref->add_day(static_cast<Days>(i + 1));
        }
    }

    pref->add_hour(static_cast<uint>(spinbox4->value()) , static_cast<uint>(spinbox5->value()));


    prof->set_preference(std::move(pref));
    // Validar y procesar datos
    if (data1.isEmpty() || data5.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor complete los campos obligatorios");
        return;
    }
    //qDebug() << "Formulario enviado:" << data1 << data2 << data3 << data4 << data5;


    dm_instance.add_professor(std::move(prof));
    this->close();
}