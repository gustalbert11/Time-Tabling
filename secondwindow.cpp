#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent, Professor* p) :
    QMainWindow(parent),
    ui(new Ui::SecondWindow)
{
    prof = p;
    ui->setupUi(this);

    // Crear widget central y layout principal
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar la ventana
    this->setWindowTitle("Formulario de Datos");
    this->setMinimumSize(600, 300);

    // Crear los campos de entrada y etiquetas
    label1 = new QLabel("Descripción 1:", this);
    lineEdit1 = new QLineEdit(this);

    label2 = new QLabel("Descripción 2:", this);
    lineEdit2 = new QLineEdit(this);

    label3 = new QLabel("Descripción 3:", this);
    lineEdit3 = new QLineEdit(this);

    label4 = new QLabel("Descripción 4:", this);
    lineEdit4 = new QLineEdit(this);

    label5 = new QLabel("Descripción 5:", this);
    lineEdit5 = new QLineEdit(this);

    // Crear layout para cada campo
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(label1);
    layout1->addWidget(lineEdit1);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(label2);
    layout2->addWidget(lineEdit2);

    QHBoxLayout *layout3 = new QHBoxLayout();
    layout3->addWidget(label3);
    layout3->addWidget(lineEdit3);

    QHBoxLayout *layout4 = new QHBoxLayout();
    layout4->addWidget(label4);
    layout4->addWidget(lineEdit4);

    QHBoxLayout *layout5 = new QHBoxLayout();
    layout5->addWidget(label5);
    layout5->addWidget(lineEdit5);

    submitButton = new QPushButton();
    QHBoxLayout *layout6 = new QHBoxLayout();
    layout6->addWidget(submitButton);

    // Agregar los layouts al layout principal
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);
    mainLayout->addLayout(layout5);
    mainLayout->addLayout(layout6);

    // Agregar un espacio elástico al final para que los campos no se expandan demasiado
    mainLayout->addStretch();

    // Configurar el widget central
    this->setCentralWidget(centralWidget);
    setupConnections();
}

void SecondWindow::setupConnections()
{
    // Conectar Enter en cada campo
    connect(lineEdit1, &QLineEdit::returnPressed, this, &SecondWindow::onFieldReturnPressed);
    connect(lineEdit2, &QLineEdit::returnPressed, this, &SecondWindow::onFieldReturnPressed);
    connect(lineEdit3, &QLineEdit::returnPressed, this, &SecondWindow::onFieldReturnPressed);
    connect(lineEdit4, &QLineEdit::returnPressed, this, &SecondWindow::onFieldReturnPressed);
    connect(lineEdit5, &QLineEdit::returnPressed, this, &SecondWindow::onFieldReturnPressed);

    connect(submitButton, &QPushButton::clicked, this, &SecondWindow::onSubmit);
    submitButton->setDefault(true);
}
SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::onFieldReturnPressed()
{
    // Mover foco al siguiente campo o enviar formulario
    QLineEdit *currentEdit = qobject_cast<QLineEdit*>(sender());

    if (currentEdit == lineEdit1) lineEdit2->setFocus();
    else if (currentEdit == lineEdit2) lineEdit3->setFocus();
    else if (currentEdit == lineEdit3) lineEdit4->setFocus();
    else if (currentEdit == lineEdit4) lineEdit5->setFocus();
    else if (currentEdit == lineEdit5) onSubmit(); // Último campo: enviar
}

void SecondWindow::onSubmit()
{
    processForm();
}

void SecondWindow::processForm()
{
    // Obtener y procesar todos los datos
    QString data1 = lineEdit1->text();
    QString data2 = lineEdit2->text();
    QString data3 = lineEdit3->text();
    QString data4 = lineEdit4->text();
    QString data5 = lineEdit5->text();

    // Validar y procesar datos
    if (data1.isEmpty() || data2.isEmpty() || data3.isEmpty() || data4.isEmpty() || data5.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor complete los campos obligatorios");
        return;
    }
    qDebug() << "Formulario enviado:" << data1 << data2 << data3 << data4 << data5;

    this->close();
}
