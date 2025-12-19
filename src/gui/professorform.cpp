#include "professorform.h"
#include "ui_professorform.h"

ProfessorForm::ProfessorForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProfessorForm)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    this->setWindowTitle("Formulario de Profesor");
    this->setMinimumSize(600, 300);

    // label1 = new QLabel("Nombre: ", this);
    // lineEdit1 = new QLineEdit(this);
    labels.push_back(new QLabel("Nombre: ", this));
    line_edits.push_back(new QLineEdit(this));

    // label3 = new QLabel("Maximo de horas diarias: ", this);
    // spinbox2 = new QSpinBox(this);
    // spinbox2->setRange(2,MAX_DAILY_HOURS);
    labels.push_back(new QLabel("Maximo de horas diarias: ", this));
    spin_boxes.push_back(new QSpinBox(this));
    spin_boxes[0]->setRange(2,MAX_DAILY_HOURS);

    // label4 = new QLabel("Maximo de horas consecutivas: ", this);
    // spinbox3 = new QSpinBox(this);
    // spinbox3->setRange(2,MAX_CONSECUTIVE_HOURS);
    labels.push_back(new QLabel("Maximo de horas consecutivas: ", this));
    spin_boxes.push_back(new QSpinBox(this));
    spin_boxes[1]->setRange(2,MAX_CONSECUTIVE_HOURS);

    //labelp = new QLabel("Preferencias\n", this);
    labels.push_back(new QLabel("Preferencias\n", this));

    // label5 = new QLabel("Descripcion:", this);
    // lineEdit5 = new QLineEdit(this);
    labels.push_back(new QLabel("Descripcion:", this));
    line_edits.push_back(new QLineEdit(this));

    // label6 = new QLabel("Tipo de preferencia:", this);
    labels.push_back(new QLabel("Tipo de preferencia:", this));
    combo_box = new QComboBox(this);

    combo_box->addItem("DAYS & HOURS",    QVariant(PreferenceType::DAYS_HOURS));
    combo_box->addItem("DAYS",       QVariant(PreferenceType::DAYS));
    combo_box->addItem("NO PREFERENCE", QVariant(PreferenceType::NO_PREFERENCE));
    combo_box->addItem("HOURS",      QVariant(PreferenceType::HOURS));

    check_boxes.push_back(new QCheckBox("MONDAY",this));
    check_boxes.push_back(new QCheckBox("TUESDAY",this));
    check_boxes.push_back(new QCheckBox("WEDNESDAY",this));
    check_boxes.push_back(new QCheckBox("THURSDAY",this));
    check_boxes.push_back(new QCheckBox("FRIDAY",this));

    // label7 = new QLabel("Hora de Inicio - Hora Final (Formato 24h): ");
    // spinbox4 = new QSpinBox(this);
    // spinbox4->setRange(MIN_START_HOUR, MAX_END_HOUR - 2);
    // spinbox5 = new QSpinBox(this);
    // spinbox5->setRange(MIN_START_HOUR + 2, MAX_END_HOUR);
    labels.push_back(new QLabel("Hora de Inicio - Hora Final (Formato 24h): "));
    spin_boxes.push_back(new QSpinBox(this));
    spin_boxes[2]->setRange(MIN_START_HOUR, MAX_END_HOUR - 2);
    spin_boxes.push_back(new QSpinBox(this));
    spin_boxes[3]->setRange(MIN_START_HOUR + 2, MAX_END_HOUR);

    QHBoxLayout *layout1 = new QHBoxLayout();
    // layout1->addWidget(label1);
    // layout1->addWidget(lineEdit1);
    layout1->addWidget(labels[0]);
    layout1->addWidget(line_edits[0]);

    QHBoxLayout *layout3 = new QHBoxLayout();
    // layout3->addWidget(label3);
    // layout3->addWidget(spinbox2);
    layout3->addWidget(labels[1]);
    layout3->addWidget(spin_boxes[0]);

    QHBoxLayout *layout4 = new QHBoxLayout();
    // layout4->addWidget(label4);
    // layout4->addWidget(spinbox3);
    layout4->addWidget(labels[2]);
    layout4->addWidget(spin_boxes[1]);

    QHBoxLayout *layoutp = new QHBoxLayout();
    //layoutp->addWidget(labelp);
    layoutp->addWidget(labels[3]);

    QHBoxLayout *layout5 = new QHBoxLayout();
    // layout5->addWidget(label5);
    // layout5->addWidget(lineEdit5);
    layout5->addWidget(labels[4]);
    layout5->addWidget(line_edits[1]);

    QHBoxLayout *layout6 = new QHBoxLayout();
    // layout6->addWidget(label6);
    layout6->addWidget(labels[5]);
    layout6->addWidget(combo_box);

    QHBoxLayout *layout8 = new QHBoxLayout();
    for(int i = 0; i < check_boxes.size(); i++)
    {
        layout8->addWidget(check_boxes[i]);
    }

    QHBoxLayout *layout9 = new QHBoxLayout();
    // layout9->addWidget(label7);
    // layout9->addWidget(spinbox4);
    // layout9->addWidget(spinbox5);
    layout9->addWidget(labels[6]);
    layout9->addWidget(spin_boxes[2]);
    layout9->addWidget(spin_boxes[3]);

    submitButton = new QPushButton(this);
    submitButton->setText("Enviar");
    QHBoxLayout *layoutl = new QHBoxLayout();
    layoutl->addWidget(submitButton);

    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);
    mainLayout->addLayout(layoutp);
    mainLayout->addLayout(layout5);
    mainLayout->addLayout(layout6);
    mainLayout->addLayout(layout8);
    mainLayout->addLayout(layout9);
    mainLayout->addLayout(layoutl);

    // Agregar un espacio elástico al final para que los campos no se expandan demasiado
    mainLayout->addStretch();

    this->setCentralWidget(centralWidget);
    setupConnections();
}

void ProfessorForm::setupConnections()
{
    // connect(lineEdit1, &QLineEdit::returnPressed, this, &ProfessorForm::onFieldReturnPressed);
    connect(line_edits[0], &QLineEdit::returnPressed, this, &ProfessorForm::onFieldReturnPressed);
    //connect(lineEdit5, &QLineEdit::returnPressed, this, &ProfessorForm::onFieldReturnPressed);
    connect(line_edits[1], &QLineEdit::returnPressed, this, &ProfessorForm::onFieldReturnPressed);
    connect(combo_box, &QComboBox::currentTextChanged, this, &ProfessorForm::combo_box_current_text_changed);
    //connect(spinbox4, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProfessorForm::start_hour_spin_box_changed);
    connect(spin_boxes[2], QOverload<int>::of(&QSpinBox::valueChanged), this, &ProfessorForm::start_hour_spin_box_changed);
    //connect(spinbox5, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProfessorForm::end_hour_spin_box_changed);
    connect(spin_boxes[3], QOverload<int>::of(&QSpinBox::valueChanged), this, &ProfessorForm::end_hour_spin_box_changed);

    connect(submitButton, &QPushButton::clicked, this, &ProfessorForm::onSubmit);
    submitButton->setDefault(true);
}

ProfessorForm::~ProfessorForm()
{
    delete ui;
}

void ProfessorForm::onFieldReturnPressed()
{
   onSubmit();
}

void ProfessorForm::combo_box_current_text_changed()
{
    if(combo_box->currentText() != "DAYS" && 
    combo_box->currentText() != "DAYS & HOURS")
    {
        for (int i = 0; i < check_boxes.size(); ++i)
        {
            check_boxes[i]->hide();
        }
    }
    else
    {
        for (int i = 0; i < check_boxes.size(); ++i)
        {
            check_boxes[i]->show();
        }
    }
    if(combo_box->currentText() != "HOURS" && 
    combo_box->currentText() != "DAYS & HOURS")
    {
        // label7->hide();
        // spinbox4->hide();
        // spinbox5->hide();
        labels[6]->hide();
        spin_boxes[2]->hide();
        spin_boxes[3]->hide();
    }
    else
    {
        // label7->show();
        // spinbox4->show();
        // spinbox5->show();
        labels[6]->show();
        spin_boxes[2]->show();
        spin_boxes[3]->show();
    }
}

void ProfessorForm::start_hour_spin_box_changed()
{
    //int start_hour = spinbox4->value();
    int start_hour = spin_boxes[2]->value();
    int min_end_hour = start_hour + 2;
    //spinbox5->setMinimum(min_end_hour);
    spin_boxes[3]->setMinimum(min_end_hour);
    // if (spinbox5->value() < min_end_hour) 
    // {
    //     spinbox5->setValue(min_end_hour);
    // }
    spin_boxes[3]->setValue(std::max(spin_boxes[3]->value(), min_end_hour));
}

void ProfessorForm::end_hour_spin_box_changed()
{
    //int end_hour = spinbox5->value();
    int end_hour = spin_boxes[3]->value();
    int max_start_hour = end_hour - 2;
    //spinbox4->setMaximum(max_start_hour);
    spin_boxes[2]->setMaximum(max_start_hour);
    // if (spinbox4->value() > max_start_hour) 
    // {
    //     spinbox4->setValue(max_start_hour);
    // }
    spin_boxes[2]->setValue(std::min(spin_boxes[2]->value(), max_start_hour));
}

void ProfessorForm::onSubmit()
{
    processForm();
}

void ProfessorForm::processForm()
{
    auto prof = std::make_unique<Professor>();

    // QString data1 = lineEdit1->text();
    QString data1 = line_edits[0]->text();
    prof->set_name(data1.toStdString());

    // uint data3 = static_cast<uint>(spinbox2->value());
    uint data3 = static_cast<uint>(spin_boxes[0]->value());
    prof->set_max_daily_hours(data3);

    // uint data4 = static_cast<uint>(spinbox3->value());
    uint data4 = static_cast<uint>(spin_boxes[1]->value());
    prof->set_max_consecutive_hours(data4);

    auto pref = std::make_unique<Preference>();
    //QString data5 = lineEdit5->text();
    QString data5 = line_edits[1]->text();
    pref->set_description(data5.toStdString());

    QString data6 = combo_box->currentText();

    pref->set_type(string_to_preference_type(data6.toStdString()));

    for (int i = 0; i < check_boxes.size(); ++i)
    {
        if(check_boxes[i]->isChecked())
        {
            pref->add_day(static_cast<Days>(i + 1));
        }
    }

    //pref->add_hour(static_cast<uint>(spinbox4->value()) , static_cast<uint>(spinbox5->value()));
    pref->add_hour(static_cast<uint>(spin_boxes[2]->value()) , static_cast<uint>(spin_boxes[3]->value()));

    prof->set_preference(std::move(pref));

    if (data1.isEmpty() || data5.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor complete los campos obligatorios");
        return;
    }
    //qDebug() << "Formulario enviado:" << data1 << data2 << data3 << data4 << data5;


    dm_instance.add_professor(std::move(prof));
    this->close();
}